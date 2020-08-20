#include <fstream>
#include <vector>
#include <algorithm>

int n;

class Work;

std::vector<Work> data;
long long minFine;
std::vector<std::vector<int>> matrixOfCases;

class Work {
private:
    int startTime, leadTime, num;
public:
    Work(const int &startTime, const int &leadTime, const int &num) : startTime(startTime), leadTime(leadTime),
                                                                      num(num) {}

    int getStart() const {
        return startTime;
    }

    int getLead() const {
        return leadTime;
    }

    int getNum() const {
        return num;
    }
};

bool greedyComp(const Work &one, const Work &two) {
    if(one.getLead() != two.getLead())
        return one.getLead() < two.getLead();
    return one.getStart() < two.getStart();
}

bool upComp(const Work &one, const Work &two) {
    return one.getStart() < two.getStart();
}

void enumeration(std::vector<int> currentSet, std::vector<bool> used, int newWork, long long currentTime, long long currentFine) {
    currentSet.push_back(newWork);
    used[newWork] = true;
    if(currentTime - data[newWork].getStart() > 0)
        currentFine += currentTime - data[newWork].getStart();
    else
        currentTime = data[newWork].getStart();
    currentTime += data[newWork].getLead();
    long long bufferFine = currentFine;
    for (int i = 0; i < n; ++i)
        if (!used[i] && currentTime - data[i].getStart() > 0)
            bufferFine += currentTime - data[i].getStart();

    if(currentFine <= minFine && bufferFine <= minFine) {
        if(currentSet.size() < n) {
            for (int i = 0; i < n; ++i)
                if (!used[i]) {
                    enumeration(currentSet, used, i, currentTime, currentFine);
                }
        }
        else {
            if(currentFine == minFine) {
                std::vector<int> buffer;
                for(int i = 0; i < n; ++i)
                    buffer.push_back(currentSet[i]);
                matrixOfCases.push_back(buffer);
            }
            else {
                minFine = currentFine;
                matrixOfCases.clear();
                std::vector<int> buffer;
                for(int i = 0; i < n; ++i)
                    buffer.push_back(currentSet[i]);
                matrixOfCases.push_back(buffer);
            }
        }
    }
}

int main() {

    std::ifstream fin("input.txt");

    fin >> n;

    int startTime, leadTime;
    for(int i = 0; i < n; ++i) {
        fin >> startTime >> leadTime;
        data.push_back(Work(startTime, leadTime, i));
    }

    fin.close();

    std::vector<Work> greedy;
    std::copy(data.begin(), data.end(), back_inserter(greedy));

    long long currentTime = 0;
    minFine = 0;

    std::sort(greedy.begin(), greedy.end(), greedyComp);
    std::sort(greedy.begin(), greedy.end(), upComp);

    for(int i = 0; i < n; ++i) {
        if(currentTime - greedy[i].getStart() > 0)
            minFine += currentTime - greedy[i].getStart();
        else
            currentTime = greedy[i].getStart();
        currentTime += greedy[i].getLead();
    }

    for(int i = 0; i < n; ++i) {
        std::vector<int> currentSet;
        std::vector<bool> used(n, false);
        enumeration(currentSet, used, i, 0, 0);
    }

    std::ofstream fout("output.txt");

    std::sort(matrixOfCases.begin(), matrixOfCases.end());

    fout << minFine<< '\n';
    for(int i = 0; i < matrixOfCases.size(); ++i) {
        for (int j = 0; j < n - 1; ++j)
            fout << (matrixOfCases[i][j] + 1) << ' ';
        fout << (matrixOfCases[i][n - 1] + 1);
        fout << '\n';
    }

    fout.close();

    return 0;
}