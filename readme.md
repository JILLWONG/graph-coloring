# 图着色问题实现情况

## 禁忌搜索

#### 实现情况

##### 部分运行结果如下表（**详细的运行日志请见“log.txt”文件**）

| 算例 | 颜色数 | 迭代次数 | 迭代时间 | 最小冲突数 |
|:-:|:-:|:-:|:-:|:-:|
| 125.1 | 5 | 1243 | 4ms | 0 |
| 125.1 | 5 | 4040 | 12ms | 0 |
| 125.1 | 5 | 1276 | 3ms | 0 |
| 125.1 | 5 | 1890 | 5ms | 0 |
| 250.1 | 8 | 16424 | 91ms | 0 |
| 250.1 | 8 | 40678 | 267ms | 0 |
| 250.1 | 8 | 21681 | 120ms | 0 |
| 250.1 | 8 | 11476 | 66ms | 0 |
| 250.5 | 28 | 469565 | 3815ms | 0 |
| 250.5 | 28 | 941754 | 7606ms | 0 |
| 250.5 | 28 | 898506 | 7047ms | 0 |
| 250.5 | 28 | 2526564 | 19846ms | 0 |
| 250.9 | 72 | 92415 | 931ms | 0 |
| 250.9 | 72 | 94691 | 1036ms | 0 |
| 250.9 | 72 | 309693 | 3055ms | 0 |
| 250.9 | 72 | 379568 | 3810ms | 0 |
| 500.1 | 12 | 26212950 | 267015ms | 0 |
| 500.1 | 12 | 46545318 | 477533ms | 0 |
| 500.1 | 12 | 83552580 | 841364ms | 0 |
| 500.5 | 50 | 1418125 | 23808ms | 0 |
| 500.5 | 50 | 3136775 | 49012ms | 0 |
| 500.5 | 49 | 54296538 | 979010ms | 0 |

500.5之前的算例可以求出最优解；250.5之前的算例可在1s内稳定求出最优解；250.5在40s内较稳定求出最优解；250.9在10s内较稳定求出最优解；500.1可在10min内求出最优解；500.5（50）可将冲突数减少到0，用时10s左右；500.5（49）可将冲突数减少到0，用时最少42s，用时较稳定，较多运行结果在1min左右，速度50k/s~60k/s.

#### 分析

前几个算例的速度较快，可达300k/s，后面的速度减慢至50k/s，原因分析如下：仇人表采用二维数组，未记录邻域动作，每次寻找最优delt值时需要遍历仇人表，搜索时间复杂度为O（N^2），随着N的增加，每次迭代耗时也增加。

#### 遗留问题

遗传算法正在实现中，经分析其时间复杂度较高，打算先通过记录邻域动作的方法提速后，再加入遗传算法。