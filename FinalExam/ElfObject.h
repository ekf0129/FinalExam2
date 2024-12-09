#ifndef ELFOBJECT_H  // ELFOBJECT_H�� ���ǵǾ� ���� ������
#define ELFOBJECT_H  // ELFOBJECT_H�� �����ϰ� �Ʒ� �ڵ带 ������

// ���� �����ϴ� ����ü
typedef struct {
    int x, y;
} Point;

// ���� �����ϴ� ����ü
typedef struct {
    Point Line[2]; // �� ������ ������ ��
} GameObject_Line;

// ���� �����ϴ� ����ü
typedef struct {
    Point Position; // ���� ��ġ
    char* Symbol;   // ���� ��ȣ
} GameObject_Dot;

// �ﰢ���� �����ϴ� ����ü
typedef struct {
    GameObject_Line Edges[3]; // �ﰢ���� �� ���� ��
} GameObject_Triangle;

#endif // ELFOBJECT_H
#pragma once
