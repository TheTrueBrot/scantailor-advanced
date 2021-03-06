// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_SPFIT_OPTIMIZER_H_
#define SCANTAILOR_SPFIT_OPTIMIZER_H_

#include <list>
#include <vector>

#include "FittableSpline.h"
#include "LinearFunction.h"
#include "MatT.h"
#include "OptimizationResult.h"
#include "QuadraticFunction.h"
#include "SqDistApproximant.h"
#include "VecNT.h"
#include "VecT.h"
#include "VirtualFunction.h"

namespace spfit {
class Optimizer {
  // Member-wise copying is OK.
 public:
  explicit Optimizer(size_t numVars = 0);

  /**
   * Sets linear constraints in the form of b^T * x + c = 0
   * Note that x in the above formula is not a vector of coordinates
   * but a vector of their displacements.  That is, the constraints
   * to be passed here depend on the current positions of control points.
   * That doesn't mean you have to provide updated constraints
   * on very iteration though, as optimize() will update them for you.
   */
  void setConstraints(const std::list<LinearFunction>& constraints);

  void addExternalForce(const QuadraticFunction& force);

  void addExternalForce(const QuadraticFunction& force, const std::vector<int>& sparseMap);

  void addInternalForce(const QuadraticFunction& force);

  void addInternalForce(const QuadraticFunction& force, const std::vector<int>& sparseMap);

  size_t numVars() const { return m_numVars; }

  /**
   * Get the external force accumulated from calls to addAttractionForce().
   * Note that optimize() will reset all forces.
   */
  double externalForce() const { return m_externalForce.c; }

  /**
   * Get the internal force accumulated from calls to addInternalForce().
   * Note that optimize() will reset all forces.
   */
  double internalForce() const { return m_internalForce.c; }

  OptimizationResult optimize(double internalExternalRatio);

  const double* displacementVector() const { return m_x.data(); }

  /**
   * Rolls back the very last adjustment to constraints done by optimize()
   * and sets the displacement vector to all zeros.
   */
  void undoLastStep();

  void swap(Optimizer& other);

 private:
  void adjustConstraints(double direction);

  size_t m_numVars;
  MatT<double> m_A;
  VecT<double> m_b;
  VecT<double> m_x;
  QuadraticFunction m_externalForce;
  QuadraticFunction m_internalForce;
};


inline void swap(Optimizer& o1, Optimizer& o2) {
  o1.swap(o2);
}
}  // namespace spfit
#endif  // ifndef SCANTAILOR_SPFIT_OPTIMIZER_H_
