#include <mh.h>

class BusquedaLocalPM : public MH {
public:
  ResultMH optimize(Problem *problem, const int maxevals) override;
};