#include "header.hpp"

namespace mania {
struct Graphics
{
    // Textures

    sf::Texture bg_tex;
    sf::Texture left_handup_tex;
    sf::Texture right_handup_tex;

    sf::Texture left_hand_tex[3];
    sf::Texture right_hand_tex[3];
    sf::Texture left_4K_tex[2];
    sf::Texture right_4K_tex[2];
    sf::Texture left_7K_tex[4];
    sf::Texture right_7K_tex[4];

    // Sprites

    sf::Sprite bg, left_handup, right_handup;

    sf::Sprite left_hand[3] = {
        sf::Sprite(left_hand_tex[0]),
        sf::Sprite(left_hand_tex[1]),
        sf::Sprite(left_hand_tex[2])
    };
    sf::Sprite right_hand[3] = {
        sf::Sprite(right_hand_tex[0]),
        sf::Sprite(right_hand_tex[1]),
        sf::Sprite(right_hand_tex[2])
    };
    sf::Sprite left_4K[2] = {
        sf::Sprite(left_4K_tex[0]), sf::Sprite(left_4K_tex[1])
    };
    sf::Sprite right_4K[2] = {
        sf::Sprite(right_4K_tex[0]), sf::Sprite(right_4K_tex[1])
    };
    sf::Sprite left_7K[4] = {
        sf::Sprite(left_7K_tex[0]),
        sf::Sprite(left_7K_tex[1]),
        sf::Sprite(left_7K_tex[2]),
        sf::Sprite(left_7K_tex[3])
    };
    sf::Sprite right_7K[4] = {
        sf::Sprite(right_7K_tex[0]),
        sf::Sprite(right_7K_tex[1]),
        sf::Sprite(right_7K_tex[2]),
        sf::Sprite(right_7K_tex[3])
    };

    Graphics() : bg(bg_tex),
      left_handup(left_handup_tex),
      right_handup(right_handup_tex)
    {}
};

Graphics gfx;

int left_key_value_4K[2], right_key_value_4K[2];
int left_key_value_7K[4], right_key_value_7K[4];
bool is_4K;

bool init() {
    // getting configs
    Json::Value mania = data::cfg["mania"];

    is_4K = mania["4K"].asBool();

    for (int i = 0; i < 2; i++) {
        left_key_value_4K[i] = mania["key4K"][i].asInt();
    }
    for (int i = 0; i < 2; i++) {
        right_key_value_4K[i] = mania["key4K"][i + 2].asInt();
    }

    for (int i = 0; i < 4; i++) {
        left_key_value_7K[i] = mania["key7K"][i].asInt();
    }
    for (int i = 0; i < 4; i++) {
        right_key_value_7K[i] = mania["key7K"][i + 3].asInt();
    }

    // importing sprites
    data::load_texture(gfx.left_handup_tex, gfx.left_handup, "img/mania/leftup.png");
    data::load_texture(gfx.right_handup_tex, gfx.right_handup, "img/mania/rightup.png");
    for (int i = 0; i < 3; i++) {
        data::load_texture(gfx.left_hand_tex[i], gfx.left_hand[i], "img/mania/left" + std::to_string(i) + ".png");
        data::load_texture(gfx.right_hand_tex[i], gfx.right_hand[i], "img/mania/right" + std::to_string(i) + ".png");
    }

    if (is_4K) {
        data::load_texture(gfx.bg_tex, gfx.bg, "img/mania/4K/bg.png");
        for (int i = 0; i < 2; i++) {
            data::load_texture(gfx.left_4K_tex[i], gfx.left_4K[i], "img/mania/4K/" + std::to_string(i) + ".png");
        }
        for (int i = 0; i < 2; i++) {
            data::load_texture(gfx.right_4K_tex[i], gfx.right_4K[i], "img/mania/4K/" + std::to_string(i + 2) + ".png");
        }
    } else {
        data::load_texture(gfx.bg_tex, gfx.bg, "img/mania/7K/bg.png");
        for (int i = 0; i < 4; i++) {
            data::load_texture(gfx.left_7K_tex[i], gfx.left_7K[i], "img/mania/7K/" + std::to_string(i) + ".png");
        }
        for (int i = 0; i < 4; i++) {
            data::load_texture(gfx.right_7K_tex[i], gfx.right_7K[i], "img/mania/7K/" + std::to_string(i + 3) + ".png");
        }
    }

    return true;
}

void draw_4K() {
    window.draw(gfx.bg);

    int left_cnt = 0, right_cnt = 0;
    int left_sum = 0, right_sum = 0;

    for (int i = 0; i < 2; i++) {
        if (input::is_pressed(left_key_value_4K[i])) {
            window.draw(gfx.left_4K[i]);
            left_cnt++;
            left_sum += i;
        }
        if (input::is_pressed(right_key_value_4K[i])) {
            window.draw(gfx.right_4K[i]);
            right_cnt++;
            right_sum += i;
        }
    }

    // draw left hand
    if (left_cnt == 0) {
        window.draw(gfx.left_handup);
    } else {
        double avg = 1.0 * left_sum / left_cnt;
        if (avg == 0) {
            window.draw(gfx.left_hand[0]);
        } else if (avg == 0.5) {
            window.draw(gfx.left_hand[1]);
        } else {
            window.draw(gfx.left_hand[2]);
        }
    }

    // draw right hand
    if (right_cnt == 0) {
        window.draw(gfx.right_handup);
    } else {
        double avg = 1.0 * right_sum / right_cnt;
        if (avg == 0) {
            window.draw(gfx.right_hand[0]);
        } else if (avg == 0.5) {
            window.draw(gfx.right_hand[1]);
        } else {
            window.draw(gfx.right_hand[2]);
        }
    }
}

void draw_7K() {
    window.draw(gfx.bg);

    int left_cnt = 0, right_cnt = 0;
    int left_sum = 0, right_sum = 0;

    for (int i = 0; i < 4; i++) {
        if (input::is_pressed(left_key_value_7K[i])) {
            window.draw(gfx.left_7K[i]);
            left_cnt++;
            left_sum += i;
        }
        if (input::is_pressed(right_key_value_7K[i])) {
            window.draw(gfx.right_7K[i]);
            right_cnt++;
            right_sum += i;
        }
    }

    // draw left hand
    if (left_cnt == 0) {
        window.draw(gfx.left_handup);
    } else {
        double avg = 1.0 * left_sum / left_cnt;
        if (avg < 1.0) {
            window.draw(gfx.left_hand[0]);
        } else if (avg <= 2.0) {
            window.draw(gfx.left_hand[1]);
        } else {
            window.draw(gfx.left_hand[2]);
        }
    }

    // draw right hand
    if (right_cnt == 0) {
        window.draw(gfx.right_handup);
    } else {
        double avg = 1.0 * right_sum / right_cnt;
        if (avg < 1.0) {
            window.draw(gfx.right_hand[0]);
        } else if (avg <= 2.0) {
            window.draw(gfx.right_hand[1]);
        } else {
            window.draw(gfx.right_hand[2]);
        }
    }
}

void draw() {
    if (is_4K) {
        draw_4K();
    } else {
        draw_7K();
    }
}
}; // namespace mania
