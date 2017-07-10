#include <iostream>
#include <queue>
#include <functional>
#include <cstring>

constexpr int CANT_GO = 114514;

int field[60][30] = {0};
int dp[2][60][30][5][3] = { 0 }; //[次の脚][左脚y][x][右相対y][x] = cost
int dx[] = {1, 1, 1, 1, 1, 2, 2, 2, 3};
int dy[] = {2, 1, 0, -1, -2, 1, 0, -1, 0};

struct Node {
	int x, y; 
	int rx, ry; //左足に対する相対
	int cost;
	bool nextR; //次に出す脚が右ならtrue
	bool operator>(const Node& rhs) const {
		if (cost == rhs.cost) return y > rhs.y;
		return cost > rhs.cost;
	}
};
int main() {
	int w, h;
	while (std::cin >> w >> h && (w || h)) {
		std::memset(dp, 0x7f, sizeof(int) * 2 * 60 * 30 * 5 * 3);
		std::vector<int> startList;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				char c; std::cin >> c;
				if ('0' <= c && c <= '9') field[i][j] = c - '0';
				else if(c == 'X') field[i][j] = CANT_GO;
				else if (c == 'S') {
					field[i][j] = 0;
					startList.push_back(j);
				}
				else if (c == 'T') field[i][j] = 0;
			}
		}

		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
		bool isOk = false;
		//全startのパターン
		for (int i = 0; i < startList.size(); i++) {
			q.push({ startList[i], h - 1, 1, 0, 0, true }); //左を乗せてる
			q.push({ startList[i] - 1, h - 1, 1, 0, 0, false }); //右を乗せてる
		}
		int minCost = 0;
		while (!q.empty() && !isOk) {
			auto node = q.top(); q.pop();
			if ((field[node.y][node.x] == 0 && node.y == 0) || (field[node.y + node.ry][node.x + node.rx] == 0 && node.y + node.ry == 0)) {
				isOk = true;
				minCost = node.cost;
				break;
			}
			for (int i = 0; i < 9; i++) {
				auto next = node;
				if (node.nextR) {
					next.rx = dx[i]; //左側を、右脚に対してOKの範囲のどれかに移動
					next.ry = dy[i];
				}
				else {
					int rx = node.x + node.rx; //左足座標
					int ry = node.y + node.ry;
					next.x = rx - dx[i]; //右足に対する左の相対座標
					next.y = ry + dy[i];
					next.rx = rx - next.x;//相対座標を出す
					next.ry = ry - next.y;
				}
				next.nextR = !node.nextR;
				//check
				if (next.x < 0 || next.x >= w || next.y < 0 || next.y >= h) continue;
				if (next.x + next.rx < 0 || next.x + next.rx >= w || next.y + next.ry < 0 || next.y + next.ry >= h) continue;
				if (!next.rx) continue;

				int cost;
				if (node.nextR) {
					cost = field[next.y + next.ry][next.x + next.rx];
				}
				else cost = field[next.y][next.x];

				if (cost < 0 || 9 < cost) continue; //Xの部分

				next.cost += cost;
				if (dp[next.nextR][next.y][next.x][next.ry + 2][next.rx - 1] <= next.cost) continue;
				dp[next.nextR][next.y][next.x][next.ry + 2][next.rx - 1] = next.cost;
				//std::cout << next.x << ", " << next.y << ", " << next.x + next.rx << ", " << next.y + next.ry << ", " << next.cost << ", " << next.nextR << std::endl;
				q.push(next);
			}
		}
		if (isOk) std::cout << minCost << std::endl;
		else std::cout << -1 << std::endl;
	}
	return 0;
}