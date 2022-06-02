# -*- coding: utf-8 -*-
"""
-------------------------------------------------
   File Name：     myapriori
   E-mail:         18109232165@163.com
   Author :        DLCT
   date：          2022/5/31
-------------------------------------------------
"""
import time

filepath = './test.txt'
set_min_support = 0.05


def readfile(path):
    """
    读取路径为path的文件，并返回数组
    """
    S = []
    length = 0
    with open(path, 'r') as INPUT:
        for line in INPUT:
            elements = line.split(' ')
            s = []
            for e in elements:

                for d in e.split():
                    s.append(int(d))
            S.append(s)
            length += 1
    return S, length


def first_candidate_item(dataset):
    """
    获得第一次候选集的集合
    return:候选集集合
    """

    C1 = []  # 存放候选集元素的数组
    for x in dataset:
        for y in x:
            if [y] not in C1:
                C1.append([y])
    C1.sort()
    print('yes')
    return C1


def get_frequent_item(dataset, c, min_support, length):
    cut_branch = {}  # 存放c中所有项集的支持度

    # 获取所有项集的支持度

    for x in c:
        for y in dataset:
            if set(x).issubset(set(y)):
                cut_branch[tuple(x)] = cut_branch.get(tuple(x), 0) + 1

    Fk = []  # 用来存放支持度大于最小支持度的项集，频繁项集
    Lk = {}
    # 删除支持度小于min_support
    # Apriori定律1，如果一个集合是频繁项集，则它的所有子集都是频繁项集
    for i in cut_branch:
        if cut_branch[i] / length >= min_support:
            Fk.append(list(i))
            Lk[i] = cut_branch[i]
    return Fk, Lk


def get_candidate(Fk, K):
    """
    求第k次候选集
    """
    Ck = []  # 存放候选集
    for i in range(len(Fk)):
        for j in range(i + 1, len(Fk)):
            L1 = list(Fk[i])[:K - 2]
            L2 = list(Fk[j])[:K - 2]
            L1.sort()
            L2.sort()  # 先排序，再进行组合
            if L1 == L2:
                if K > 2:
                    new = list(set(Fk[i]) ^ set(Fk[j]))
                else:
                    new = set()
                for x in Fk:
                    # new包含在Fk[i] 和 Fk[j]中没有同时出现的
                    # Apriori定律2：如果一个集合不是频繁项集，则它的所有超集都不是频繁项集
                    if set(new).issubset(set(x)) and list(
                            set(Fk[i]) | set(Fk[j])) not in Ck:  # 减枝 new是 x 的子集，并且 还没有加入 ck 中

                        Ck.append(list(set(Fk[i]) | set(Fk[j])))
    return Ck


def Apriori(dataset, min_support, length):
    C1 = first_candidate_item(dataset)
    f1, L1 = get_frequent_item(dataset=dataset, c=C1, min_support=min_support, length=length)
    F = [f1]  # 最终项集集合
    L = L1  # 最终项集及其支持度

    K = 2

    while len(F[K - 2]) > 1:  # 前一集合的长度 大于1才运行:
        Ck = get_candidate(F[K - 2], K)  # 获取第k次候选集
        fk, Lk = get_frequent_item(dataset=dataset, c=Ck, min_support=min_support, length=length)
        # 获取第k个候选集的频繁项集，及其支持度字典
        F.append(fk)
        L.update(Lk)

        print('第' + str(K) + '次产生的候选集：')
        print(Ck)
        print('第' + str(K) + '次产生的频繁集：')
        print(Lk)

        K += 1
    return F, L


# def generateRules(sup_data, min_cof=0.1):
#     relus = {}
#     for item in sup_data:
#         if len(item) == 1:
#             continue
#         if sup_data[item] / sup_data[(item[0], )] >= min_cof:
#             relus[item] = sup_data[item] / sup_data[(item[0], )]
#     return relus


if __name__ == '__main__':
    t1 = time.time()
    dataset, length = readfile(path=filepath)
    F, L = Apriori(dataset=dataset, min_support=set_min_support, length=length)
    # relus = generateRules(L)
    print('最终得出的对应支持度大于规定支持度的具有关联的商品是：')
    for item in L:
        if len(item) == 1:
            continue
        for i in range(len(item)):
            if i != 0:
                print('->', end="")
            print(item[i], end="")
        print('  支持度为: ' + str(L.get(item) / length))
    t2 = time.time()
    print(int(round(t2 - t1)))
