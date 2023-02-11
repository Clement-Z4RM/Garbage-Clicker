/*
** EPITECH PROJECT, 2022
** JAM
** File description:
** play
*/

#include "../../include/epi_jam.h"

static void handle_waste_bags(jam_t *jam, sfVector2i pos, unsigned int *fails)
{
    sfTime time = sfClock_getElapsedTime(jam->jam_p.clock);

    for (u_int i = 0; jam->jam_p.waste_bags[i].sprite; ++i) {
        jam->jam_p.waste_bags[i].pos.x = (((time.microseconds / 2000) - 250) + time.microseconds / 8000);
        sfSprite_setPosition(jam->jam_p.vacuum, sfVector2i_to_sfVector2f(pos));
        sfSprite_setPosition(jam->jam_p.waste_bags[i].sprite, jam->jam_p.waste_bags[i].pos);
        waste_bags_pick_up(jam, sfVector2i_to_sfVector2f(pos), fails, i);
    }
}

static void draw_elements(jam_t *jam, char *score, char *fails, unsigned int *fails_nb)
{
    sfRenderWindow_clear(jam->window, sfBlack);
    sfRenderWindow_drawSprite(jam->window, jam->game_background, NULL);
    for (u_int i = 0; jam->jam_p.waste_bags[i].sprite; ++i)
        sfRenderWindow_drawSprite(jam->window, jam->jam_p.waste_bags[i].sprite, NULL);
    sfRenderWindow_drawSprite(jam->window, jam->jam_p.vacuum, NULL);
    sfRenderWindow_drawText(jam->window, jam->jam_p.score_, NULL);
    sfRenderWindow_drawText(jam->window, jam->jam_p.fail_, NULL);
    sfText_setString(jam->jam_p.score, my_int_to_str(jam->score, score));
    sfText_setString(jam->jam_p.fail, my_int_to_str((*fails_nb), fails));
    sfRenderWindow_drawText(jam->window, jam->jam_p.score, NULL);
    sfRenderWindow_drawText(jam->window, jam->jam_p.fail, NULL);
}

static int leave_play(jam_t *jam, unsigned int err, unsigned int fails)
{
    // TODO replace 500 5 or another raisonable number
    if (sfKeyboard_isKeyPressed(sfKeyEscape) == sfTrue || fails > 500) {
        if (jam->highest_score < jam->score)
            jam->highest_score = jam->score;
        if (jam->all_time_best_score < jam->score) {
            jam->all_time_best_score = jam->score;
            all_time_best_score(jam);
        }
        err = (render_title_screen(jam) == 84) ? (84) : (0);
    }
    return (err);
}

static int render_play(jam_t *jam)
{
    unsigned int err = 0;
    unsigned int fails = 0;
    sfVector2i pos = {0};
    char *score = malloc(sizeof(char) * 21);
    char *fails_char = malloc(sizeof(char) * 21);

    jam->score = 0;
    for (u_int i = 0; jam->jam_p.waste_bags[i].sprite; ++i) {
        // TODO change this line
        jam->jam_p.waste_bags[i].pos = (sfVector2f){(rand() % 300 * -1), (rand() % 880)}; // {rand() % 1920, -100}; fait spawn le bag en haut de l'écran à une position x random
        sfSprite_setPosition(jam->jam_p.waste_bags[i].sprite, jam->jam_p.waste_bags[i].pos);
    }
    while (sfRenderWindow_isOpen(jam->window)) {
        check_closing_event(jam);
        draw_elements(jam, score, fails_char, &fails);
        pos = sfMouse_getPositionRenderWindow(jam->window);
        handle_waste_bags(jam, pos, &fails);
        err = leave_play(jam, err, fails);
        sfRenderWindow_display(jam->window);
    }
    free(score);
    return (err);
}

int play(jam_t *jam)
{
    // sfRenderWindow_setMouseCursorVisible(jam->window, sfFalse);
    sfClock_restart(jam->jam_p.clock);
    if (render_play(jam) == 84)
        return (84);
    return (0);
}
