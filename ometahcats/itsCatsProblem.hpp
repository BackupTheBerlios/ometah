#ifndef ITSCATSPROBLEM
#define ITSCATSPROBLEM

#include <ometah/itsProblem.hpp>
#include <ometah/common/itsPoint.hpp>


class itsCatsProblem : public itsProblem
{
public:

    itsCatsProblem();
    itsPoint objectiveFunction(itsPoint point);
};

class itsCatsProblemFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};

#endif
