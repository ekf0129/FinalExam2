#ifndef ELFOBJECT_H  // ELFOBJECT_H가 정의되어 있지 않으면
#define ELFOBJECT_H  // ELFOBJECT_H를 정의하고 아래 코드를 컴파일

// 점을 정의하는 구조체
typedef struct {
    int x, y;
} Point;

// 선을 정의하는 구조체
typedef struct {
    Point Line[2]; // 두 점으로 구성된 선
} GameObject_Line;

// 점을 정의하는 구조체
typedef struct {
    Point Position; // 점의 위치
    char* Symbol;   // 점의 기호
} GameObject_Dot;

// 삼각형을 정의하는 구조체
typedef struct {
    GameObject_Line Edges[3]; // 삼각형의 세 개의 선
} GameObject_Triangle;

#endif // ELFOBJECT_H
#pragma once
