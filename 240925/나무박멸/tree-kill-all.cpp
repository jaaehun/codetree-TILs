#include<iostream>
#include<string.h>

using namespace std;

int n, k, c;
int map[21][21];
int copy_map[21][21];
int destroy_map[21][21];
int sum;
int flag;

int dy[4] = {0,0,-1,1};
int dx[4] = {-1,1,0,0};
int dyy[4] = {-1,-1,1,1};
int dxx[4] = {-1,1,-1,1};

void grow_tree() {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] > 0){
                int cnt = 0;
                for(int d=0;d<4;d++){
                    int now_y = i + dy[d];
                    int now_x = j + dx[d];

                    if(now_y >= 0 && now_y < n && now_x >= 0 && now_x < n){
                        if(map[now_y][now_x] > 0){
                            cnt++;
                        }
                    }
                }
                map[i][j] += cnt;
            }
        }
    }
}

void spread_tree() {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] > 0){
                int cnt = 0;
                int tree = map[i][j];
                for(int d=0;d<4;d++){
                    int now_y = i + dy[d];
                    int now_x = j + dx[d];

                    if(now_y >= 0 && now_y < n && now_x >= 0 && now_x < n){
                        if(map[now_y][now_x] == 0){
                            cnt++;
                        }
                    }
                }
                if(cnt > 0){
                    int new_tree = tree / cnt;
                    for(int d=0;d<4;d++){
                        int now_y = i + dy[d];
                        int now_x = j + dx[d];

                        if(now_y >= 0 && now_y < n && now_x >= 0 && now_x < n){
                            if(map[now_y][now_x] == 0){
                                copy_map[now_y][now_x] += new_tree;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(copy_map[i][j] > 0){
                map[i][j] = copy_map[i][j];
            }
        }
    }
    memset(copy_map,0,sizeof(copy_map));
}

void destroy_tree() {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] > 0){
                int cnt = 0;
                cnt += map[i][j];
                for(int d=0;d<4;d++){
                    for(int s=1;s<=k;s++){
                        int now_y = i + s*dyy[d];
                        int now_x = j + s*dxx[d];

                        if(now_y >= 0 && now_y < n && now_x >= 0 && now_x < n){
                            if(map[now_y][now_x] > 0){
                                cnt += map[now_y][now_x];
                            }else{
                                break;
                            }
                        }else{
                            break;
                        }
                    }
                }
                destroy_map[i][j] = cnt;
            }
        }
    }
    int max_cnt = -1;
    int des_y = 0;
    int des_x = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] > 0){
                if(destroy_map[i][j] > max_cnt){
                    max_cnt = destroy_map[i][j];
                    des_y = i;
                    des_x = j;
                }else if(destroy_map[i][j] == max_cnt){
                    if(i < des_y){
                        des_y = i;
                        des_x = j;
                    }else if(i == des_y){
                        if(j < des_x){
                            des_y = i;
                            des_x = j;
                        }
                    }
                }
            }
        }
    }
    if(max_cnt < 0){
        flag = 1;
        return;
    }
    sum += map[des_y][des_x];
    map[des_y][des_x] = -c-1;
    for(int d=0;d<4;d++){
        for(int s=1;s<=k;s++){
            int now_y = des_y + s*dyy[d];
            int now_x = des_x + s*dxx[d];

            if(now_y >= 0 && now_y < n && now_x >= 0 && now_x < n){
                if(map[now_y][now_x] > 0){
                    sum += map[now_y][now_x];
                    map[now_y][now_x] = -c-1;
                }else{
                    if(map[now_y][now_x] != -100){
                        map[now_y][now_x] = -c-1;
                    }
                    break;
                }
            }else{
                break;
            }      
        } 
    }
    memset(destroy_map,0,sizeof(destroy_map));
}

void heal_tree() {
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] < 0 && map[i][j] != -100){
                map[i][j]++;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int m;
    cin >> n >> m >> k >> c;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin >> map[i][j];
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j] == -1){
                map[i][j] = -100;
            }
        }
    }
    sum = 0;
    for(int t=0;t<m;t++){
        flag = 0;
        grow_tree();
        spread_tree();
        destroy_tree();
        if(flag == 1){
            break;
        }
        heal_tree();
    }
    cout << sum;

    return 0;
}