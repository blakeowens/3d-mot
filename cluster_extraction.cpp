#include <iostream>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/conversions.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>

using namespace std;

ros::Publisher pub;

void cloud_cb(const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {

    // Container for original & filtered data.
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_f(new pcl::PointCloud<pcl::PointXYZ>);

    // Convert to PCL data type.
    pcl::fromROSMsg(*cloud_msg, *cloud);

    // Create the filtering object: downsample the dataset using a leaf size of 1cm
    pcl::VoxelGrid<pcl::PointXYZ> vg;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
    vg.setInputCloud(cloud);
    vg.setLeafSize(0.1f, 0.1f, 0.1f);
    vg.filter(*cloud_filtered);

    // Create the segmentation object for the planar model and set all the parameters
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ> ());
    pcl::PCDWriter writer;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(100);
    seg.setDistanceThreshold(0.2);

    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud(cloud_filtered);
    seg.segment(*inliers, *coefficients);
    if (inliers->indices.size() > 0) {

        // Extract the planar inliers from the input cloud
        pcl::ExtractIndices<pcl::PointXYZ> extract;
        extract.setInputCloud(cloud_filtered);
        extract.setIndices(inliers);
        extract.setNegative(false);

        // Get the points associated with the planar surface
        extract.filter(*cloud_plane);

        // Remove the planar inliers, extract the rest
        extract.setNegative(true);
        extract.filter(*cloud_f);
        *cloud_filtered = *cloud_f;

    }

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr color_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::copyPointCloud(*cloud_filtered, *color_cloud);

    // Creating the KdTree object for the search method of the extraction
    pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
    tree->setInputCloud(color_cloud);

    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZRGB> ec;
    ec.setClusterTolerance(0.25); // .25m
    ec.setMinClusterSize(50);
    ec.setMaxClusterSize(25000);
    ec.setSearchMethod(tree);
    ec.setInputCloud(color_cloud);
    ec.extract(cluster_indices);

    // Output to ROS topic.
    const int NUM_COLORS = 5;
    int color_r[NUM_COLORS] = {255, 0, 0, 255, 0};
    int color_g[NUM_COLORS] = {0, 255, 0, 255, 255};
    int color_b[NUM_COLORS] = {0, 0, 255, 0, 255};
    int j = 0;

    for(std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it) {
        for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); ++pit) {
            color_cloud->points[*pit].r = color_r[j%NUM_COLORS];
            color_cloud->points[*pit].g = color_g[j%NUM_COLORS];
            color_cloud->points[*pit].b = color_b[j%NUM_COLORS];
        }

        // Convert to ROS data type.
        sensor_msgs::PointCloud2 output;
        pcl::toROSMsg(*color_cloud, output);

        // Publish the data.
        pub.publish(output);

        ++j;
    }

}

int main(int argc, char *argv[]) {

    // Initialize ROS.
    ros::init(argc, argv, "cluster_extraction");

    // Check arguments.
    if(argc != 3) {
        ROS_ERROR("Usage: rosrun my_pcl_tutorial cluster_extraction [input topic]");
        return 1;
    }

    // Create a ROS subscriber for the input point cloud.
    ROS_INFO("Subscribing to %s...", argv[1]);
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe(argv[1], 1, cloud_cb);

    // Create a ROS publisher for the output point cloud
    const char *advertise = "/cluster_output";
    ROS_INFO("Advertising topic %s...", advertise);
    pub = nh.advertise<sensor_msgs::PointCloud2>(advertise,1);

    ROS_INFO("Ready.");

    // Spin
    ros::spin();

    return 0;

}
