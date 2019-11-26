//DPLKYL002
/**
 (
 (     )\ )
 ( )\   (()/(   (    ) (        (        (  (
 )((_)   /(_)) ))\( /( )(   (   )\  (    )\))(
 ((_)_   (_))  /((_)(_)|()\  )\ |(_) )\ )((_))\
 / _ \  | |  (_))((_)_ ((_)_(_/((_)_(_/( (()(_)
 | (_) | | |__/ -_) _` | '_| ' \)) | ' \)) _` |
 \__\_\ |____\___\__,_|_| |_||_||_|_||_|\__, |
 |___/

 Refer to Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
 for a detailed discussion on Q Learning
 */
#include "CQLearningController.h"

using namespace std;

CQLearningController::CQLearningController(HWND hwndMain) :
		CDiscController(hwndMain), _grid_size_x(
				CParams::WindowWidth / CParams::iGridCellDim + 1), _grid_size_y(
				CParams::WindowHeight / CParams::iGridCellDim + 1) {
}

template<typename ReturnType, typename ContainerType>
ReturnType findMax(const vector<ContainerType>& max) {

	return *std::max_element(max.begin(), max.end());

}

int findReward(const vector<double>& reward) {

	double max = findMax<double, double>(reward);

	vector < uint > position;

	for (int k = 0; k < reward.size(); ++k) {

		if (reward[k] == max) {

			position.push_back(k);

		}

	}

	return position[RandInt(0, position.size() - 1)];
}

/**
 The update method should allocate a Q table for each sweeper (this can
 be allocated in one shot - use an offset to store the tables one after the other)

 You can also use a boost multiarray if you wish
 */
void CQLearningController::InitializeLearningAlgorithm(void) {

	//TODO

	for (uint k = 0; k < _grid_size_x; ++k) {

		vector<vector<double>> s;

		for (uint p = 0; p < _grid_size_y; ++p) {

			s.push_back( { 0, 0, 0, 0 });

		}

		_Q_s_a.push_back(s);

	}
}

/**
 The immediate reward function. This computes a reward upon achieving the goal state of
 collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and
 of course for hitting supermines/rocks!
 */
double CQLearningController::R(uint x, uint y, uint sweeper_no) {
	//TODO: roll your own here!

	double rewardValue = -10;

	int nearestObject = (m_vecSweepers[sweeper_no])->CheckForObject(
			m_vecObjects, CParams::dMineScale);

	if (nearestObject > -1) {

		switch (m_vecObjects[nearestObject]->getType()) {

		case CCollisionObject::Mine: {

			if (!m_vecObjects[nearestObject]->isDead()) {

				rewardValue = 100;

			}

			break;

		}
		case CCollisionObject::Rock: {

			rewardValue = -50;

			break;

		}
		case CCollisionObject::SuperMine: {

			rewardValue = -500;

			break;

		}

		}

	}

	return rewardValue;

}

/**
 The update method. Main loop body of our Q Learning implementation
 See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
 */
bool CQLearningController::Update(void) {

	uint cDead = std::count_if(m_vecSweepers.begin(), m_vecSweepers.end(),
			[](CDiscMinesweeper * s)->bool {
				return s->isDead ();
			});
	if (cDead == CParams::iNumSweepers) {
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
	}

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw) {
		if (m_vecSweepers[sw]->isDead())
			continue;

		/**
		 Q-learning algorithm according to:
		 Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		 */
		//1:::Observe the current state:
		//TODO

		auto index = m_vecSweepers[sw]->Position();

		index /= 10;

		//2:::Select action with highest historic return:
		//TODO

		int largestReward = findReward(_Q_s_a[index.x][index.y]);

		m_vecSweepers[sw]->setRotation(ROTATION_DIRECTION(largestReward));
		//now call the parents update, so all the sweepers fulfill their chosen action
	}
	CDiscController::Update(); //call the parent's class update. Do not delete this.

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw) {
		if (m_vecSweepers[sw]->isDead())
			continue;

		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		//3:::Observe new state:
		//TODO

		auto indexPrev = m_vecSweepers[sw]->PrevPosition();

		indexPrev /= 10;

		auto indexCurrent = m_vecSweepers[sw]->Position();

		indexCurrent /= 10;

		vector<double> vectIndexPrev = _Q_s_a[indexPrev.x][indexPrev.y];

		vector<double> vectIndexCurrent = _Q_s_a[indexCurrent.x][indexCurrent.y];

		auto a = m_vecSweepers[sw]->getRotation();

		//4:::Update _Q_s_a accordingly:
		//TODO

		double value = learningRate
				* (R(indexCurrent.x, indexCurrent.y, sw)
						+ (discountFactor
								* (findMax<double, double>(vectIndexCurrent)))
						- vectIndexPrev[a]);

		_Q_s_a[indexPrev.x][indexPrev.y][a] += value;
	}
	return true;
}

CQLearningController::~CQLearningController(void) {

	//TODO: dealloc stuff here if you need to	

}


