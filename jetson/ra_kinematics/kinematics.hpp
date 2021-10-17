#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <eigen3/Eigen/Dense>
#include "arm_state.hpp"
#include <stack>

using namespace Eigen;

typedef Matrix<double, 6, 1> Vector6d;

static constexpr int MAX_ITERATIONS = 500;
static constexpr int MAX_ITERATIONS_LOW_MOVEMENT = 10;

// The acceptable distance from a solution to the target
static constexpr double POS_THRESHOLD = 0.05;
static constexpr double ANGLE_THRESHOLD = 0.02;

// The percentage of the remaining distance to try to move
static constexpr double k_position_step = 0.1;
static constexpr double k_angle_step = 0.24;

// The amount to change an angle to find the corresponding change in euler angles
static constexpr double DELTA_THETA = 0.0001;

static constexpr double EPSILON_DIST = 0.0000000001;                //TODO: Experiment with epsilon values
static constexpr double EPSILON_ANGLE_DIST = 0.0000000001;

class KinematicsSolver {

private:

    bool e_locked;
    int num_iterations;

    stack<vector<double>> arm_state_backup;

    /**
     * Push the angles of robot_state into the arm_state_backup stack
     * */
    void perform_backup(ArmState &robot_state);

    /**
     * Pop a backup of the angles from robot_state and copy them into robot_state
     * Then run FK to resolve state
     * */
    void recover_from_backup(ArmState &robot_state);

    Matrix4d apply_joint_xform(const ArmState &robot_state, const string &joint, double theta);
    Matrix4d get_joint_xform(const ArmState &robot_state, const string &joint, double theta);

    void IK_step(ArmState &robot_state, const Vector6d &d_ef, bool use_euler_angles);

    /**
     * called by is_safe to check that angles are within bounds
     * @param angles the set of angles for a theoretical arm position
     * @return true if all angles are within bounds
     * */
    bool limit_check(ArmState &robot_state, const vector<double> &angles);

public:

    KinematicsSolver();

    void FK(ArmState &robot_state);

    pair<Vector6d, bool> IK(ArmState &robot_state, const Vector6d &target_point, bool set_random_angles, bool use_euler_angles);

    /**
     * @param angles the set of angles for a theoretical arm position
     * @return true if all angles are within bounds and don't cause collisions
     * */
    bool is_safe(ArmState &robot_state, const vector<double> &angles);

    int get_num_iterations();

};


#endif
