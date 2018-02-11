// Copyright (c) 2018 FRC Team 3512. All Rights Reserved.

/* Implementation of trapezoid motion profile translated to C++; base Java code
 * courtesy of FRC Team 254
 */

/* Constant acceleration until target (max) velocity is reached, sets
 * acceleration to zero for a calculated time, then decelerates at a constant
 * acceleration with a slope equal to the negative slope of the initial
 * acceleration.
 */

#ifndef LIVEGRAPHER_TEST_SRC_TRAPEZOIDPROFILE_HPP_
#define LIVEGRAPHER_TEST_SRC_TRAPEZOIDPROFILE_HPP_

#include "ProfileBase.hpp"

class TrapezoidProfile : public ProfileBase {
public:
    TrapezoidProfile(double maxV, double timeToMaxV);
    virtual ~TrapezoidProfile();

    /* If distance:
     *     curSetpoint is current distance set to which to travel
     *     curSource is current position (not used)
     *
     * If velocity:
     *     curSetpoint is current velocity at which to travel
     *     curSource is current velocity (not used)
     *
     * curTime is current time
     */
    virtual double updateSetpoint(double curTime, double curSource = 0.0);

    /* goal is a distance to which to travel
     * curSource is the current position
     * t initializes m_lastTime
     */
    virtual double setGoal(double t, double goal, double curSource = 0.0);

    void setMaxVelocity(double v);
    double getMaxVelocity() const;
    void setTimeToMaxV(double timeToMaxV);

protected:
    double m_acceleration;
    double m_velocity;
    double m_profileMaxVelocity;
    double m_timeFromMaxVelocity;
    double m_timeToMaxVelocity;
    double m_sign;
};

#endif  // LIVEGRAPHER_TEST_SRC_TRAPEZOIDPROFILE_HPP_
