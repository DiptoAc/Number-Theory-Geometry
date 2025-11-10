#include <bits/stdc++.h>
using namespace std;
#define pi acos(-1)
#define int long long
struct Point {
    long long x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

// Cross product (p0p1) x (p0p2), computed in 128-bit to avoid overflow.
static inline long long cross(const Point& p0, const Point& p1, const Point& p2) {
    __int128 x1 = (__int128)p1.x - p0.x;
    __int128 y1 = (__int128)p1.y - p0.y;
    __int128 x2 = (__int128)p2.x - p0.x;
    __int128 y2 = (__int128)p2.y - p0.y;
    __int128 c = x1 * y2 - y1 * x2;
    if (c > 0) return 1;
    if (c < 0) return -1;
    return 0;
}

static inline long long dist2(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return (dx*dx + dy*dy); // fits in signed 64-bit up to ~1e9 coords
}

// Graham's scan: returns convex hull in CCW order, starting at the lowest (y, then x).
// If all points are collinear, returns the two endpoints (sorted by (x,y)).
vector<Point> graham_scan(vector<Point> pts) {
    int n = (int)pts.size();
    if (n <= 1) return pts;

    // 1) Remove exact duplicates
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){
        return (a.y < b.y) || (a.y == b.y && a.x < b.x);
    });
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = (int)pts.size();
    if (n <= 1) return pts;

    // 2) Choose pivot: lowest y then lowest x (already at pts[0] after sort)
    Point pivot = pts[0];

    // 3) Sort by polar angle w.r.t. pivot (CCW). For ties (same angle),
    //    keep the farthest one *last* so inner collinears get discarded later.
    sort(pts.begin() + 1, pts.end(), [&](const Point& a, const Point& b){
        long long c = cross(pivot, a, b);
        if (c != 0) return c > 0;                // a before b if pivot->a is CCW of pivot->b
        // collinear with pivot: put closer first, farther later
        return dist2(pivot, a) < dist2(pivot, b);
    });

    // 4) Build the hull
    vector<Point> st;
    st.reserve(n);
    st.push_back(pivot);

    for (int i = 1; i < n; ++i) {
        // Skip points identical to previous (already deduped globally, but safe)
        while (st.size() >= 2 && cross(st[st.size()-2], st.back(), pts[i]) <= 0) {
            // <= 0 means we remove right turns and collinear points on the same line,
            // which, together with sorting farthest-last, keeps only outer boundary.
            st.pop_back();
        }
        st.push_back(pts[i]);
    }



    return st;
    /// ALL CODES BELOW IS JUST ORNAMENTS.

    // Special case: all points collinear -> keep just two endpoints
    if (st.size() > 2) return st;


    // Determine global min and max along the line
    Point lo = pts.front(), hi = pts.front();
    for (auto &p : pts) {
        if (p.x < lo.x || (p.x == lo.x && p.y < lo.y)) lo = p;
        if (p.x > hi.x || (p.x == hi.x && p.y > hi.y)) hi = p;
    }
    if (lo == hi) return {lo};       // all points identical
    return {lo, hi};
}



int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int i,j,k,tc,cs=0;
    cin >> tc;
    while(tc--)
    {
        cout << "Case " << ++cs << ": ";
        cin >> n;
        vector<Point> pts(n);
        for (i = 0; i < n; ++i)
        {
            cin >> pts[i].x >> pts[i].y;
        }
        auto hull = graham_scan(pts);
        int mod = hull.size();
        if(mod < 3){cout << 0.0 << endl;continue;}
        // Output: |hull| followed by the points in CCW order
        //cout << hull.size() << "\n";for (auto &p : hull){cout << p.x << " " << p.y << "\n";}
        double mn = pi;
        for(i=0; i<hull.size(); i++)
        {
            Point ba,bc;

            ba.x = hull[(i-1+mod)%mod].x - hull[(i+mod)%mod].x;
            ba.y = hull[(i-1+mod)%mod].y - hull[(i+mod)%mod].y;

            bc.x = hull[(i+1+mod)%mod].x - hull[(i+mod)%mod].x;
            bc.y = hull[(i+1+mod)%mod].y - hull[(i+mod)%mod].y;

            double absba = ba.x*ba.x + ba.y*ba.y;
            double absbc = bc.x*bc.x + bc.y*bc.y;
            absba = sqrtl(absba);
            absbc = sqrtl(absbc);

            int dot = ba.x * bc.x + ba.y * bc.y;
            double val = (double)dot/(double)(absba * absbc);

            double angle = acos(val);
            mn = min(angle,mn);

        }
        double degree = mn*180.0/pi;
        cout << fixed << setprecision(10) << degree << endl;

    }
}
