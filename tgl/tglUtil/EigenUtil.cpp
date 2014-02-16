/*
 * EigenUtil.cpp
 */

#include "EigenUtil.h"

namespace tgl {

Eigen::Matrix3d rotFromRpy(double r, double p, double y)
{
    const double cr = cos(r);
    const double sr = sin(r);
    const double cp = cos(p);
    const double sp = sin(p);
    const double cy = cos(y);
    const double sy = sin(y);

    Eigen::Matrix3d R;
    R << cp*cy, sr*sp*cy - cr*sy, cr*sp*cy + sr*sy,
         cp*sy, sr*sp*sy + cr*cy, cr*sp*sy - sr*cy,
         -sp  , sr*cp           , cr*cp;

    return R;
}

Eigen::Vector3d omegaFromRot(const Eigen::Matrix3d& R)
{
    double alpha = (R(0,0) + R(1,1) + R(2,2) - 1.0) / 2.0;

    if(fabs(alpha - 1.0) < 1.0e-6) {   //th=0,2PI;
        return Eigen::Vector3d::Zero();

    } else {
        double th = acos(alpha);
        double s = sin(th);

        if (s < std::numeric_limits<double>::epsilon()) {   //th=PI
            return Eigen::Vector3d( sqrt((R(0,0)+1)*0.5)*th, sqrt((R(1,1)+1)*0.5)*th, sqrt((R(2,2)+1)*0.5)*th );
        }

        double k = -0.5 * th / s;

        return Eigen::Vector3d((R(1,2) - R(2,1)) * k,
                       (R(2,0) - R(0,2)) * k,
                       (R(0,1) - R(1,0)) * k);
    }
}

} /* namespace tgl */
