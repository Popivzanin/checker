#include "testlib.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

constexpr int minN = 2, maxN = 300'000;
constexpr int minK = 2, maxK = 300'000;

class Input{
public:
    int n{};
    int k{};

    Input(InStream& stream) : n(stream.readInt(minN, maxN, "n")), k(stream.readInt(minK, maxK, "k")) {}
};

void indices_duplicate_check(int max_index, std::vector<int>& v, InStream& source) {
    std::vector<int> used(max_index + 1, -1);
    for(int i = 0; i < static_cast<int>(v.size()); ++i){
        int index { v[i] };
        source.quitif(used[index] != -1, _wa, "duplicate indices %d at positions %d and %d", index, i + 1, used[index]);
        used[index] = i + 1;
    }
}

int binpow(int n, int a, int k){
    if(n == 0)
        return 1;
    if(n & 1)
        return a * 1ll * binpow(n - 1, a, k) % k;
    return binpow(n / 2, a * 1ll * a % k, k);
}

bool read_answer(Input& input, InStream& stream){
    std::string exists { stream.readToken("YES|NO", "exists") };
    if(exists == "NO")
        return 0;

    int n { input.n };
    int k { input.k };

    std::vector<int> p {stream.readInts(n, 1, n, "p")};
    indices_duplicate_check(n, p, stream);

    std::vector<int> inv(k);
    for(int i = 0; i < k; ++i)
        inv[i] = binpow(k - 2, i, k);

    std::vector<int> b_counts(k, 0);
    for(int i = 1; i <= n; ++i){
        int cur_rest = i * 1ll * inv[p[i - 1] % k] % k;
        ++b_counts[cur_rest];
    }

    for(int i = 0; i < k; ++i)
        stream.quitif(b_counts[i] != n / k, _wa,
            "Expected permutation B, but elements with reminder %d modulo %d don't occur precisely %d / %d times", i, k, n, k);

    return 1;
}

bool read_answer_from(Input& input, InStream& stream, const std::string& source){
    bool answer { read_answer(input, stream) };
    stream.quitif(!stream.seekEof(), _pe, "Extra information in the %s file", source.c_str());
    return answer;
}

int main(int argc, char * argv[])
{
    registerTestlibCmd(argc, argv);

    Input input(inf);

    bool ja { read_answer_from(input, ans, "answer") };
    bool pa { read_answer_from(input, ouf, "output") };

    if(ja && !pa)
        ouf.quitf(_wa, "permutation exists, but participant did find it");
    if(!ja && pa)
        ans.quitf(_wa, "permutation exists, but jury did find it");
    if(ja && pa)
        quitf(_ok, "OK, correct permutation was found");
    if(!ja && !pa)
        quitf(_ok, "OK, permutation doesn't exist");

    return 0;
}
