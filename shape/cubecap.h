#ifndef CUBECAP_H
#define CUBECAP_H

#include "cap.h"
class CubeCap : public Cap
{
public:
    CubeCap();
    ~CubeCap();
   virtual std::vector<float> buildShape(int numEdges, float y, float radius, bool direction);
};

#endif // CUBECAP_H
