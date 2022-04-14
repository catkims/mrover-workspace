#pragma once

#include <vector>
#include <optional>

#include <eigen3/Eigen/Core>

#include "rover.hpp"
#include "filter.hpp"
#include "cache.hpp"
#include "rover_msgs/Obstacle.hpp"
#include "rover_msgs/TargetList.hpp"

using Eigen::Vector2d;
using namespace rover_msgs;

class Rover;

class Environment {
private:
    Obstacle mObstacle{0.0, 0.0, -1.0};

    // Reference to config variables
    const rapidjson::Document& mConfig;

    Cache<Target> mTargetLeft, mTargetRight;

    Filter<double> mPostOneLat, mPostOneLong, mPostTwoLat, mPostTwoLong;

    bool mHasNewPostUpdate = false;

    int mBaseGateId{};

public:
    explicit Environment(const rapidjson::Document& config);

    Obstacle getObstacle();

    void setObstacle(Obstacle const& obstacle);

    void setBaseGateID(int baseGateId);

    void setTargets(TargetList const& targets);

    void updateTargets(std::shared_ptr<Rover> const& rover, std::shared_ptr<CourseProgress> const& course);

    [[nodiscard]] Target getLeftTarget() const;

    [[nodiscard]] Target getRightTarget() const;

    [[nodiscard]] Odometry getPostOneLocation() const;

    [[nodiscard]] Odometry getPostTwoLocation() const;

    [[nodiscard]] Vector2d getPostOneOffsetInCartesian(Odometry cur) const;

    [[nodiscard]] Vector2d getPostTwoOffsetInCartesian(Odometry cur) const;

    [[nodiscard]] bool hasNewPostUpdate() const;

    [[nodiscard]] bool hasGateLocation() const;

    [[nodiscard]] bool hasPostOneLocation() const;

    [[nodiscard]] bool hasPostTwoLocation() const;

    [[nodiscard]] std::optional<Target> tryGetTargetWithId(int32_t id) const;
};