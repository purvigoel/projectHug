#ifndef SPHERECAP_H
#define SPHERECAP_H

#include "cap.h"
class SphereCap : public Cap
{
public:
    SphereCap();
    std::vector<float> buildShape(int numEdges, float y, float radius, bool direction);
};

#endif // SPHERECAP_H
