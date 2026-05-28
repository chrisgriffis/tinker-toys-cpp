#pragma once

namespace Griffis
{
    class Helper
    {
    public:
        static const unsigned SCALENE = 1;
        static const unsigned ISCOCELES = 2;
        static const unsigned EQUILATERAL = 3;
        static const unsigned ERROR = 4;

        static unsigned TriangleTest(int A, int B, int C)
        {
            unsigned retVal = ERROR;

            if(A < 1 || B < 1 || C < 1)
            {
                retVal = ERROR;
            }
            else
            {
                bool AequalsB(A == B);
                bool BequalsC(B == C);
                bool CequalsA(C == A);

                if (AequalsB || BequalsC || CequalsA)
                {
                    if (AequalsB && BequalsC && CequalsA)
                    {
                        retVal = EQUILATERAL;
                    } 
                    else
                    {
                        retVal = ISCOCELES;
                    }
                } 
                else
                {
                    retVal = SCALENE;
                }
            }
            return retVal;
        }
    };
}