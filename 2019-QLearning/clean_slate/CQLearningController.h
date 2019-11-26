//DPLKYL002
#pragma once
#include "cdisccontroller.h"
#include "CParams.h"
#include "CDiscCollisionObject.h"
#include <cmath>
#include <algorithm>
#include <vector>

typedef unsigned int uint;
class CQLearningController: public CDiscController {
private:
	uint _grid_size_x;
	uint _grid_size_y;

	const double discountFactor = 0.9;

	const double learningRate = 0.5;

	std::vector<std::vector<std::vector<double>>> _Q_s_a;

public:
	CQLearningController(HWND hwndMain);
	virtual void InitializeLearningAlgorithm(void);
	double R(uint x, uint y, uint sweeper_no);
	virtual bool Update(void);
	virtual ~CQLearningController(void);
};
