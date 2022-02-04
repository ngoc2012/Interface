#ifndef VECTORF_H // include guard
#define VECTORF_H

#include <vector>
#include <algorithm>

using namespace std;

namespace vectorF  // namespace declaration
{
    // the compiler can't include complete versions of template functions => must be define in header file
    template<class T>
    bool isInVector(vector<T> vec, T item) {
        return (find(vec.begin(), vec.end(), item) != vec.end());
    }

    template<class T>
    void checkSize(vector<T> vec, int i, const string indexName, const string vectorName) {
        if (i > vec.size()) {
            cout << indexName << " index " << i << " is bigger than " << vectorName << "'s size " << endl;
            exit(0);
        }
    }
}

#endif