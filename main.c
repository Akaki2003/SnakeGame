#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

const int unit = 20;
int gameSpeed = 0;
const int ScreenWidth = 1200;
const int ScreenHeight = 720;

enum RectDirection {
	LEFT,
	RIGHT,
	DOWN,
	UP
};

struct Snake
{
	struct Snake* next;
	struct SDL_Rect* rect;
	enum RectDirection rectDir;
};

struct Snake* IncreaseSize(struct Snake* oldTail) {
	SDL_Rect* rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	struct Snake* tail = (struct Snake*)malloc(sizeof(struct Snake));
	tail->rect = rect;
	tail->next = NULL;
	tail->rect->h = unit;
	tail->rect->w = unit;
	tail->rectDir = oldTail->rectDir;
	oldTail->next = tail;
	switch (oldTail->rectDir)
	{
	case DOWN:
		tail->rect->x = oldTail->rect->x;
		tail->rect->y = oldTail->rect->y - unit;
		break;
	case UP:
		tail->rect->x = oldTail->rect->x;
		tail->rect->y = oldTail->rect->y + unit;
		break;
	case LEFT:
		tail->rect->y = oldTail->rect->y;
		tail->rect->x = oldTail->rect->x + unit;
		break;
	case RIGHT:
		tail->rect->y = oldTail->rect->y;
		tail->rect->x = oldTail->rect->x - unit;
		break;

	default:
		break;
	}
	return tail;
}

void SnakeRender(struct Snake* head, struct SDL_Rect food, SDL_Renderer* renderer) {
	struct Snake* current = head;
	while (current) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, current->rect);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, current->rect);
		current = current->next;
	}
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &food);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}

SDL_Rect FoodGenerator() {
	int x = ((rand() % ((ScreenWidth - 200) / unit)) * unit) + 200;
	int y = ((rand() % ((ScreenHeight - 200) / unit)) * unit) + 200;

	SDL_Rect newRect;
	newRect.h = unit;
	newRect.w = unit;
	newRect.x = x;
	newRect.y = y;
	return newRect;
}


void ShowText(SDL_Renderer* renderer, const char* text, int x, int y, int w, int h) {
	TTF_Font* Sans = TTF_OpenFont("OpenSans-Italic-VariableFont_wdth,wght.ttf", 40);

	SDL_Color White = { 255, 255, 255 };


	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(Sans, text, White);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.w = w;
	Message_rect.h = h;
	Message_rect.x = x;
	Message_rect.y = y;
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

int main(int argc, char** argv) {

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_CreateWindowAndRenderer(ScreenWidth, ScreenHeight, 0, &window, &renderer);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect;
	SDL_Rect rect2;
	SDL_Rect rect3;
	int score = 0;
	char countText[60];


	SDL_Rect food = FoodGenerator();

	enum RectDirection headRectDir = RIGHT;
	struct Snake head = {NULL,&rect,RIGHT };

	struct Snake tail = {NULL,&rect2,RIGHT };
	struct Snake realtail = {NULL,&rect3,RIGHT };
	struct Snake* tailPointer = &realtail;


	head.next = &tail;
	head.rect = &rect;
	tail.rect = &rect2;
	tail.next = &realtail;
	rect.w = unit;
	rect.h = unit;

	head.rect->y = 220;
	head.rect->x = 260;

	rect2.w = unit;
	rect2.h = unit;

	head.next->rect->y = 220;
	head.next->rect->x = 240;

	rect3.w = unit;
	rect3.h = unit;

    tail.next->rect->y = 220;
    tail.next->rect->x = 220;

	int x = 0;
	int y = 0;

	bool quit = false;
	bool quitScreen = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT || (e.type == SDL_MOUSEBUTTONDOWN)) {
				if (e.type == SDL_QUIT) {
					quitScreen = true;
				}
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (headRectDir == DOWN) break;
					headRectDir = UP;
					break;
				case SDLK_DOWN:
					if (headRectDir == UP) break;
					headRectDir = DOWN;
					break;
				case SDLK_RIGHT:
					if (headRectDir == LEFT) break;
					headRectDir = RIGHT;
					break;
				case SDLK_LEFT:
					if (headRectDir == RIGHT) break;
					headRectDir = LEFT;
					break;
				default:
					break;
				}
				head.rectDir = headRectDir;
				break;
				//SDL_FlushEvent(SDL_KEYDOWN);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		struct Snake* current = &head;
		int prevx = current->rect->x;
		int prevy = current->rect->y;
		enum RectDirection prevDir = current->rectDir;

		switch (headRectDir)
		{
		case UP:
			head.rect->y -= unit;
			break;

		case DOWN:
			head.rect->y += unit;
			break;

		case LEFT:
			head.rect->x -= unit;
			break;

		case RIGHT:
			head.rect->x += unit;
			break;

		default:
			break;
		}

		if (head.rect->x > ScreenWidth) {
			head.rect->x = 0;
		}
		else if (head.rect->x < 0) {
			head.rect->x = ScreenWidth;
		}
		else if (head.rect->y > ScreenHeight) {
			head.rect->y = 0;
		}
		else if (head.rect->y < 0) {
			head.rect->y = ScreenHeight;
		}

		int currentx = 0;
		int currenty = 0;
		enum RectDirection currentDir = head.rectDir;
		while (current->next)
		{
			currentx = current->next->rect->x;
			currenty = current->next->rect->y;
			currentDir = current->next->rectDir;
			current->next->rect->x = prevx;
			current->next->rect->y = prevy;
			current->next->rectDir = prevDir;
			current = current->next;
			prevx = currentx;
			prevy = currenty;
			if (head.rect->x == current->rect->x && head.rect->y == current->rect->y) {
				quit = true;
			}
			prevDir = currentDir;
		}

		if (head.rect->x == food.x && head.rect->y == food.y) {
			food = FoodGenerator();
			tailPointer = IncreaseSize(tailPointer);
			score++;
			if (score % 3 == 0 && gameSpeed < 70) {
				gameSpeed += 5;
			}
		}
		SnakeRender(&head, food, renderer);

		sprintf(countText, "Score: %d", score);
		ShowText(renderer, countText, 20, 20, 70, 70);

		SDL_RenderPresent(renderer);

		SDL_Delay(100 - gameSpeed);
	}


	while (!quitScreen) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT || (e.type == SDL_MOUSEBUTTONDOWN)) {
				quitScreen = true;
			}
			ShowText(renderer, "You have lost", ScreenWidth / 2 - 300 / 2, ScreenHeight / 2 - 300 / 2, 300, 300);
			SDL_RenderPresent(renderer);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();

	return EXIT_SUCCESS;
}
