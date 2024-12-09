#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>  // _getch()�� ����ϱ� ���� ���
#include <windows.h> // Sleep() �Լ��� ����ϱ� ���� ���
#include <time.h> // �ð� ���� �Լ� ����� ���� ���

#include "ElfObject.h"  // ElfObject.h�� �����Ͽ� ����ü ���Ǹ� ���

#define WIDTH 40  // ȭ�� �ʺ�
#define HEIGHT 24 // ȭ�� ����
#define PI 3.14159265 // ������ ���

// ȭ�� ���� �ʱ�ȭ �Լ�
void clearBuffer(char* buffer, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        buffer[i] = ' '; // ȭ�� ���۸� �������� �ʱ�ȭ
    }
}

// ���� ��� �Լ�
void printBuffer(char* buffer, int width, int height) {
    system("cls"); // �ܼ� ȭ���� ����� (Windows������ ����)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            putchar(buffer[y * width + x]); // ������ ������ ���
        }
        putchar('\n'); // �� �ٲ�
    }
}

// �� �׸��� �Լ�
void drawLine(char* buffer, int width, int height, GameObject_Line line) {
    // ���� �������� ���� ���� ���̸� ���
    int dx = abs(line.Line[1].x - line.Line[0].x);
    int dy = abs(line.Line[1].y - line.Line[0].y);
    int sx = (line.Line[0].x < line.Line[1].x) ? 1 : -1; // x ������ ���� �Ǵ� ����
    int sy = (line.Line[0].y < line.Line[1].y) ? 1 : -1; // y ������ ���� �Ǵ� ����
    int err = dx - dy; // ���� �ʱ�ȭ

    while (1) {
        if (line.Line[0].x >= 0 && line.Line[0].x < width && line.Line[0].y >= 0 && line.Line[0].y < height) {
            buffer[line.Line[0].y * width + line.Line[0].x] = '@'; // ���� �׸���
        }

        if (line.Line[0].x == line.Line[1].x && line.Line[0].y == line.Line[1].y)
            break; // ���� ���� �����ϸ� ����

        int e2 = err * 2; // ���� ������ �� ��
        if (e2 > -dy) {
            err -= dy; // y �������� �̵�
            line.Line[0].x += sx; // x �������� �̵�
        }
        if (e2 < dx) {
            err += dx; // x �������� �̵�
            line.Line[0].y += sy; // y �������� �̵�
        }
    }
}

// ���� �׸��� �Լ�
void drawDot(char* buffer, int width, int height, GameObject_Dot dot) {
    int x = dot.Position.x; // ���� x ��ǥ
    int y = dot.Position.y; // ���� y ��ǥ

    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = '+'; // ���� ��ġ�� '+' ǥ��
        buffer[y * width + x + 1] = '+'; // ���� �����ʿ� '+' ǥ��
    }
}

// �ﰢ���� �׸��� �Լ�
void drawTriangle(char* buffer, int width, int height, GameObject_Line* triangle) {
    for (int i = 0; i < 3; i++) {
        drawLine(buffer, width, height, triangle[i]); // �ﰢ���� �� ���� �׸���
    }
}

// �ﰢ�� ȸ�� �Լ� (��� �̿�)
void rotateTriangle(GameObject_Line* triangle, float angle) {
    float rad = angle * (PI / 180.0); // ������ �������� ��ȯ
    float cosA = cos(rad); // �ڻ��� ��
    float sinA = sin(rad); // ���� ��

    for (int i = 0; i < 3; i++) {
        // ������ �������� ȸ��
        int x = triangle[i].Line[0].x - 20; // �߽��� (20, 12)�� ����
        int y = triangle[i].Line[0].y - 12;

        // ȸ�� ��ȯ (���)
        triangle[i].Line[0].x = (int)(x * cosA - y * sinA) + 20; // ȸ�� �� x ��ǥ
        triangle[i].Line[0].y = (int)(x * sinA + y * cosA) + 12; // ȸ�� �� y ��ǥ

        x = triangle[i].Line[1].x - 20; // �� ��° �� ȸ��
        y = triangle[i].Line[1].y - 12;

        // ȸ�� ��ȯ (���)
        triangle[i].Line[1].x = (int)(x * cosA - y * sinA) + 20;
        triangle[i].Line[1].y = (int)(x * sinA + y * cosA) + 12;
    }
}

// �ﰢ�� �̵� �Լ�
void moveTriangle(GameObject_Line* triangle, int direction) {
    // direction: 1 = ���� (����), -1 = ���� (�Ʒ���)
    int dy = direction; // y �������� 1px �̵�

    // �ﰢ���� ������ ��ġ ������Ʈ
    for (int i = 0; i < 3; i++) {
        triangle[i].Line[0].y += dy; // ù ��° ���� y ��ǥ �̵�
        triangle[i].Line[1].y += dy; // �� ��° ���� y ��ǥ �̵�
    }
}

// ���� �Լ�
int main() {
    char* buffer = malloc(WIDTH * HEIGHT + 1); // ȭ�� ���� �޸� �Ҵ� (+1�� null terminator��)

    // �ﰢ���� �����ϴ� �� ����
    GameObject_Line triangle[3] = {
        {{{20, 5}, {10, 15}}}, // ���� ��
        {{{20, 5}, {30, 15}}}, // ������ ��
        {{{10, 15}, {30, 15}}}  // �غ�
    };

    // �ð� ���� ����
    clock_t lastTime = clock(); // ������ �ð� �ʱ�ȭ

    while (1) {
        clearBuffer(buffer, WIDTH, HEIGHT); // ���� �ʱ�ȭ
        drawTriangle(buffer, WIDTH, HEIGHT, triangle); // �ﰢ�� �׸���

        // �ﰢ���� ��� �������� �� �׸���
        GameObject_Dot dot = { {triangle[0].Line[0].x, triangle[0].Line[0].y}, "++" }; // ������ ��ġ
        drawDot(buffer, WIDTH, HEIGHT, dot); // �� �׸���

        printBuffer(buffer, WIDTH, HEIGHT); // ȭ�鿡 ���

        // Ű �Է� ���
        if (_kbhit()) {
            char ch = _getch(); // Ű �Է� �ޱ�
            if (ch == 'a') {
                rotateTriangle(triangle, 1); // 'a' Ű�� ������ 1�� ���� ȸ��
            }
            else if (ch == 'd') {
                rotateTriangle(triangle, -1); // 'd' Ű�� ������ 1�� ���� ȸ��
            }
            else if (ch == 's') {
                moveTriangle(triangle, 1); // 's' Ű�� ������ 1px �Ʒ��� �̵�
            }
            else if (ch == 'w') {
                moveTriangle(triangle, -1); // 'w' Ű�� ������ 1px ���� �̵�
            }
            else if (ch == 'q') {
                break; // 'q' Ű�� ������ ����
            }
        }

        // 30�������� ���� ��� �ð� ����
        Sleep(33); // �� 33ms ���
    }

    free(buffer); // �Ҵ��� �޸� ����
    return 0;
}
