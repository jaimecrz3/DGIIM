#include <mh.h>

class BusquedaLocalMejor : public MH {
public:
  ResultMH optimize(Problem *problem, const int maxevals) override;
};