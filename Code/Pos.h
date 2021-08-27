// Position struct for holding coordinate information for a vertex 
#ifndef POS_H
#define POS_H

class Pos {

    public:

        int vertexVal;
        double x;
        double y;
        double z;

        Pos()
        {
            vertexVal = 0;
            x = 0;
            y = 0;
            z = 0;
        }

        Pos (int a, double x1, double y1, double z1)
        {
            vertexVal = a;
            x = x1;
            y = y1;
            z = z1;
        }

        Pos(const Pos &other)
        {
            vertexVal = other.vertexVal;
            x = other.x;
            y = other.y;
            z = other.z;
            // ...
        }
        Pos &operator=(const Pos &t)
        {
            vertexVal = t.vertexVal;
            x = t.x;
            y = t.y;
            z = t.z;
            return *this; //...
        }

}; //end class 

#endif