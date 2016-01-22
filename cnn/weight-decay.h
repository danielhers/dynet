#ifndef CNN_WEIGHT_DECAY_H
#define CNN_WEIGHT_DECAY_H

#include <stdexcept>
#include <cmath>

namespace cnn {

// I don't bother with learning rates when computing how much the weight
// decay changes- those are hard to define in the adaptive update rules.
// So, we do something simple that works with everything.
//
// Note: you may want to discount lambda as you learn if your eta is on a
// decaying schedule.
struct L2WeightDecay {
  explicit L2WeightDecay(float lambda = 1e-6) : weight_decay(1) { SetLambda(lambda); }
  void SetLambda(float lam) {
    if (lam < 0) throw std::domain_error("Bad value of lambda in SetLambda");
    lambda = lam;
  }
  void UpdateWeightDecay(unsigned num_updates = 1) {
    if (num_updates == 0) return;
    if (num_updates == 1)
      weight_decay -= weight_decay * lambda;
    else weight_decay = weight_decay * std::pow(1-lambda, num_updates);
  }
  float CurrentWeightDecay() const { return weight_decay; }
  bool ParametersNeedRescaled() const {
    return (weight_decay < 0.25f);
  }
  void ResetWeightDecay() { weight_decay = 1.0f; }
 private:
  float weight_decay;
  float lambda;
};

extern L2WeightDecay global_weight_decay;

} // namespace cnn

#endif
