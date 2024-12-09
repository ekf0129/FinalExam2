#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>  // _getch()를 사용하기 위한 헤더
#include <windows.h> // Sleep() 함수를 사용하기 위한 헤더
#include <time.h> // 시간 관련 함수 사용을 위한 헤더

#include "ElfObject.h"  // ElfObject.h를 포함하여 구조체 정의를 사용

#define WIDTH 40  // 화면 너비
#define HEIGHT 24 // 화면 높이
#define PI 3.14159265 // 원주율 상수

// 화면 버퍼 초기화 함수
void clearBuffer(char* buffer, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        buffer[i] = ' '; // 화면 버퍼를 공백으로 초기화
    }
}

// 버퍼 출력 함수
void printBuffer(char* buffer, int width, int height) {
    system("cls"); // 콘솔 화면을 지우기 (Windows에서만 동작)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            putchar(buffer[y * width + x]); // 버퍼의 내용을 출력
        }
        putchar('\n'); // 줄 바꿈
    }
}

// 선 그리기 함수
void drawLine(char* buffer, int width, int height, GameObject_Line line) {
    // 선의 시작점과 끝점 간의 차이를 계산
    int dx = abs(line.Line[1].x - line.Line[0].x);
    int dy = abs(line.Line[1].y - line.Line[0].y);
    int sx = (line.Line[0].x < line.Line[1].x) ? 1 : -1; // x 방향의 증가 또는 감소
    int sy = (line.Line[0].y < line.Line[1].y) ? 1 : -1; // y 방향의 증가 또는 감소
    int err = dx - dy; // 오차 초기화

    while (1) {
        if (line.Line[0].x >= 0 && line.Line[0].x < width && line.Line[0].y >= 0 && line.Line[0].y < height) {
            buffer[line.Line[0].y * width + line.Line[0].x] = '@'; // 선을 그리기
        }

        if (line.Line[0].x == line.Line[1].x && line.Line[0].y == line.Line[1].y)
            break; // 선의 끝에 도달하면 종료

        int e2 = err * 2; // 현재 오차의 두 배
        if (e2 > -dy) {
            err -= dy; // y 방향으로 이동
            line.Line[0].x += sx; // x 방향으로 이동
        }
        if (e2 < dx) {
            err += dx; // x 방향으로 이동
            line.Line[0].y += sy; // y 방향으로 이동
        }
    }
}

// 점을 그리는 함수
void drawDot(char* buffer, int width, int height, GameObject_Dot dot) {
    int x = dot.Position.x; // 점의 x 좌표
    int y = dot.Position.y; // 점의 y 좌표

    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = '+'; // 점의 위치에 '+' 표시
        buffer[y * width + x + 1] = '+'; // 점의 오른쪽에 '+' 표시
    }
}

// 삼각형을 그리는 함수
void drawTriangle(char* buffer, int width, int height, GameObject_Line* triangle) {
    for (int i = 0; i < 3; i++) {
        drawLine(buffer, width, height, triangle[i]); // 삼각형의 각 변을 그리기
    }
}

// 삼각형 회전 함수 (행렬 이용)
void rotateTriangle(GameObject_Line* triangle, float angle) {
    float rad = angle * (PI / 180.0); // 각도를 라디안으로 변환
    float cosA = cos(rad); // 코사인 값
    float sinA = sin(rad); // 사인 값

    for (int i = 0; i < 3; i++) {
        // 원점을 기준으로 회전
        int x = triangle[i].Line[0].x - 20; // 중심을 (20, 12)로 설정
        int y = triangle[i].Line[0].y - 12;

        // 회전 변환 (행렬)
        triangle[i].Line[0].x = (int)(x * cosA - y * sinA) + 20; // 회전 후 x 좌표
        triangle[i].Line[0].y = (int)(x * sinA + y * cosA) + 12; // 회전 후 y 좌표

        x = triangle[i].Line[1].x - 20; // 두 번째 점 회전
        y = triangle[i].Line[1].y - 12;

        // 회전 변환 (행렬)
        triangle[i].Line[1].x = (int)(x * cosA - y * sinA) + 20;
        triangle[i].Line[1].y = (int)(x * sinA + y * cosA) + 12;
    }
}

// 삼각형 이동 함수
void moveTriangle(GameObject_Line* triangle, int direction) {
    // direction: 1 = 전진 (위로), -1 = 후진 (아래로)
    int dy = direction; // y 방향으로 1px 이동

    // 삼각형의 꼭지점 위치 업데이트
    for (int i = 0; i < 3; i++) {
        triangle[i].Line[0].y += dy; // 첫 번째 점의 y 좌표 이동
        triangle[i].Line[1].y += dy; // 두 번째 점의 y 좌표 이동
    }
}

// 메인 함수
int main() {
    char* buffer = malloc(WIDTH * HEIGHT + 1); // 화면 버퍼 메모리 할당 (+1은 null terminator용)

    // 삼각형을 구성하는 선 정의
    GameObject_Line triangle[3] = {
        {{{20, 5}, {10, 15}}}, // 왼쪽 선
        {{{20, 5}, {30, 15}}}, // 오른쪽 선
        {{{10, 15}, {30, 15}}}  // 밑변
    };

    // 시간 관련 변수
    clock_t lastTime = clock(); // 마지막 시간 초기화

    while (1) {
        clearBuffer(buffer, WIDTH, HEIGHT); // 버퍼 초기화
        drawTriangle(buffer, WIDTH, HEIGHT, triangle); // 삼각형 그리기

        // 삼각형의 상단 꼭지점에 점 그리기
        GameObject_Dot dot = { {triangle[0].Line[0].x, triangle[0].Line[0].y}, "++" }; // 꼭지점 위치
        drawDot(buffer, WIDTH, HEIGHT, dot); // 점 그리기

        printBuffer(buffer, WIDTH, HEIGHT); // 화면에 출력

        // 키 입력 대기
        if (_kbhit()) {
            char ch = _getch(); // 키 입력 받기
            if (ch == 'a') {
                rotateTriangle(triangle, 1); // 'a' 키를 누르면 1도 좌측 회전
            }
            else if (ch == 'd') {
                rotateTriangle(triangle, -1); // 'd' 키를 누르면 1도 우측 회전
            }
            else if (ch == 's') {
                moveTriangle(triangle, 1); // 's' 키를 누르면 1px 아래로 이동
            }
            else if (ch == 'w') {
                moveTriangle(triangle, -1); // 'w' 키를 누르면 1px 위로 이동
            }
            else if (ch == 'q') {
                break; // 'q' 키를 누르면 종료
            }
        }

        // 30프레임을 위한 대기 시간 설정
        Sleep(33); // 약 33ms 대기
    }

    free(buffer); // 할당한 메모리 해제
    return 0;
}
