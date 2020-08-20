#include "iostream"
#include "queue"
#include "algorithm"

class Land {
private:
    int a;
    int b;
    bool visit;
public:
    Land() : a(INT32_MAX), b(0), visit(false) {}
    void setA(const int &a) {
        this->a = a;
    }
    void setB(const int &b) {
        this->b = b;
    }
    void setVisit(const bool &visit) {
        this->visit = visit;
    }
    int getA() const {
        return a;
    }
    int getB() const {
        return b;
    }
    bool getVisit() const {
        return visit;
    }
};

class State {
private:
    int landNum;
    int time;
public:
    State(const int &landNum, const int &time) : landNum(landNum), time(time) {}
    State(const State &state) : landNum(state.landNum), time(state.time) {}
    int getLandNum() const {
        return landNum;
    }
    int getTime() const {
        return time;
    }
};

bool canGo(Land land, int t) {
    int time = t % (land.getA() + land.getB());
    if(time == 0)
        return false;
    if(time <= land.getA())
        return true;
    return false;
}

int NOD(int a, int b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

int NOK(int a, int b) {
    return a / NOD(a, b) * b;
}

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, t = 0, maxT = 3000;
    scanf("%d", &n);

    Land river[n + 2];
    for(int i = 1; i <= n; ++i) {
        int a, b;
        scanf("%d", &a);
        scanf("%d", &b);
        river[i].setA(a);
        river[i].setB(b);
        //maxT = NOK(maxT, a + b);
    }

    std::queue<State> way;
    int currentLand = 0, currentT = 0;
    bool canFinish = false;

    way.push(State(currentLand, 0));

    while(!canFinish && !way.empty()) {
        State state = way.front();

        if(state.getTime() <= maxT) {
            if (currentT < state.getTime() + 1) {
                for (int i = 1; i <= n; i++)
                    river[i].setVisit(false);
                ++currentT;
            }

            int x = state.getLandNum() - 5, y = state.getLandNum() + 5;
            if (x < 0)
                x = 0;
            if (y > n + 1)
                y = n + 1;

            for (int i = x; i <= y; i++) {
                if (i == 0 && state.getTime() + 1 < maxT)
                    way.push(State(0, state.getTime() + 1));
                else {
                    if (i >= 1 && i <= n && river[i].getVisit() == false && canGo(river[i], state.getTime() + 1) &&
                        state.getTime() + 1 < maxT) {
                        way.push(State(i, state.getTime() + 1));
                        river[i].setVisit(true);
                    } else if (i == n + 1 && state.getTime() <= maxT) {
                        t = state.getTime() + 1;
                        canFinish = true;
                        break;
                    }
                }
            }
        }

        way.pop();
    }

    if(canFinish)
        printf("%d", t);
    else
        printf("No");

    return 0;
}