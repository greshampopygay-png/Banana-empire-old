#include <genesis.h>

// 1. ДАННЫЕ ИГРЫ (Твои переменные)
long score = 0;
int click_power = 1;
int auto_power = 0;
int click_cost = 15;
int auto_cost = 50;
int is_vip = 0; // 0 = Обычный, 1 = VIP x2 режим

char text_buffer[32]; 

// Функция обновления интерфейса на экране
void update_interface() {
    // Главный счетчик бананов
    sprintf(text_buffer, "BANANAS: %ld      ", score);
    VDP_drawText(text_buffer, 11, 5);

    // Текущая сила клика и автокликер
    int current_click = is_vip ? (click_power * 2) : click_power;
    sprintf(text_buffer, "Click: +%d | Auto: +%d  ", current_click, auto_power);
    VDP_drawText(text_buffer, 6, 7);

    // Магазин улучшений
    sprintf(text_buffer, "1. Power-up Cost: %d    ", click_cost);
    VDP_drawText(text_buffer, 4, 14);
    
    sprintf(text_buffer, "2. Banan-Bot Cost: %d   ", auto_cost);
    VDP_drawText(text_buffer, 4, 16);

    // Если игрок нажал кнопку и стал VIP — выводим жирный статус на экране
    if (is_vip) {
        VDP_drawText("★ VIP STATUS ACTIVATED: X2 ★", 3, 2);
    } else {
        VDP_drawText("  [Press START for Free VIP]  ", 2, 2);
    }
}

int main() {
    VDP_setTextPlane(BG_A);

    // Красивое текстовое меню
    VDP_drawText("--- BANANA EMPIRE 16-BIT ---", 6, 0);
    VDP_drawText("      xeoni7 Official       ", 6, 1);
    
    // Наш текстовый банан по центру
    VDP_drawText("     _     ", 14, 9);
    VDP_drawText("   // \\\\   ", 14, 10);
    VDP_drawText("  ((   ))  ", 14, 11);
    VDP_drawText("   \\\\_//   ", 14, 12);

    // Инструкция по управлению для игрока
    VDP_drawText("CONTROLS:", 4, 19);
    VDP_drawText("(A)     - Click Banana", 4, 21);
    VDP_drawText("(B)     - Buy Power-up", 4, 22);
    VDP_drawText("(C)     - Buy Banan-Bot", 4, 23);
    VDP_drawText("(START) - GET FREE VIP X2!", 4, 25);

    update_interface();

    u32 last_auto_tick = 0;
    u16 last_joy_state = 0;

    // ИГРОВОЙ ЦИКЛ СЕГИ
    while(1) {
        u16 joy = JOY_readJoypad(JOY_1);

        // 1. КЛИК НА КНОПКУ А (Работает с учетом VIP)
        if ((joy & BUTTON_A) && !(last_joy_state & BUTTON_A)) {
            int p = is_vip ? (click_power * 2) : click_power;
            score += p;
            update_interface();
        }

        // 2. ПРОКАЧКА КЛИКА НА КНОПКУ B
        if ((joy & BUTTON_B) && !(last_joy_state & BUTTON_B)) {
            if (score >= click_cost) {
                score -= click_cost;
                click_power++;
                click_cost = click_cost * 16 / 10;
                update_interface();
            }
        }

        // 3. ПОКУПКА БОТА НА КНОПКУ C
        if ((joy & BUTTON_C) && !(last_joy_state & BUTTON_C)) {
            if (score >= auto_cost) {
                score -= auto_cost;
                auto_power++;
                auto_cost = auto_cost * 17 / 10;
                update_interface();
            }
        }

        // 4. ТА САМАЯ КНОПКА VIP! Нажимаешь START — и ты VIP x2!
        if ((joy & BUTTON_START) && !(last_joy_state & BUTTON_START)) {
            if (!is_vip) {
                is_vip = 1; // Включаем VIP
                update_interface(); // Сразу перерисовываем экран
            }
        }

        last_joy_state = joy;

        // РАБОТА АВТОКЛИКЕРА
        u32 current_time = SYS_getTick();
        if (current_time - last_auto_tick >= 60) {
            if (auto_power > 0) {
                score += auto_power;
                update_interface();
            }
            last_auto_tick = current_time;
        }

        SYS_doVBlankProcess();
    }

    return 0;
}
    
