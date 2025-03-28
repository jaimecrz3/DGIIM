#include <mh.h>

class BruteSearch : public MH {
public:
  ResultMH optimize(Problem *problem, const int maxevals) override;
};