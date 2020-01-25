/// \file
/// \brief Library for calculating the velocities to move between waypoints

#include <iostream>
#include <cmath>
#include "rigid2d/rigid2d.hpp"
#include "rigid2d/diff_drive.hpp"
#include "rigid2d/waypoints.hpp"

namespace rigid2d
{
  Waypoints::Waypoints()
  {
    rate = 10;
    linv_lim = 5;
    angv_lim = 5;

    point_list.at(0) = Vector2D (1,1);
    point_list.at(1) = Vector2D (5,1);
    point_list.at(2) = Vector2D (5,5);
    point_list.at(3) = Vector2D (3,6);
    point_list.at(4) = Vector2D (1,5);

    target = point_list.at(0);

    point_list.push_back(target);
  }

  Waypoints::Waypoints(std::vector<Vector2D> points)
  {
    rate = 10;
    linv_lim = 5;
    angv_lim = 5;

    point_list = points;
  }

  void Waypoints::getTarget()
  {
    target = point_list.at(0);
    point_list.erase(point_list.begin());
    point_list.push_back(target);
  }

  Twist2D Waypoints::nextWaypoint(Pose2D pos)
  {
    Twist2D tw;
    double x_dist, y_dist;
    double calc_dist, calc_heading;
    double err_heading;
    double dist_thresh = 0.05, ang_thresh = 0.05;

    tw.vy = 0;

    x_dist = target.x - pos.x;
    y_dist = target.y - pos.y;

    // calculate the linear distance between the robot and target
    calc_dist = std::hypot(x_dist, y_dist);

    // calculate the angular distance between the robot and target
    calc_heading = std::atan2(y_dist, x_dist);
    err_heading = calc_heading - pos.th;

    // update target if nessissary
    if(calc_dist < dist_thresh)
    {
      getTarget();
    }

    if(err_heading < ang_thresh)
    {
      // move straight
      tw.wz = 0;
      tw.vx = linv_lim;
    }

    else
    {
      // turn
      tw.wz = angv_lim;
      tw.vx = 0;
    }

    //return the twist
    return tw;
  }

  void Waypoints::setVlims(double lin, double ang)
  {
    linv_lim = lin;
    angv_lim = ang;
  }

  void Waypoints::setRate(int r)
  {
    rate = r;
  }
}
