#include "header.hpp"

namespace taiko {
Json::Value rim_key_value[2], centre_key_value[2];
struct Graphics{
    sf::Texture bg_tex;
    sf::Texture up_tex[3], rim_tex[2], centre_tex[2];
    sf::Sprite bg;
    sf::Sprite up[2] = {
        sf::Sprite(up_tex[0]), sf::Sprite(up_tex[1])
    };
    sf::Sprite rim[2] = {
        sf::Sprite(rim_tex[0]), sf::Sprite(rim_tex[1])
    };
    sf::Sprite centre[2] = {
        sf::Sprite(centre_tex[0]), sf::Sprite(centre_tex[1])
    };

    Graphics() : bg(bg_tex){}
};

Graphics gfx;

int key_state[2] = {0, 0};
bool rim_key_state[2] = {false, false};
bool centre_key_state[2] = {false, false};
double timer_rim_key[2] = {-1, -1};
double timer_centre_key[2] = {-1, -1};

bool init() {
    // getting configs
    bool chk[256];
    std::fill(chk, chk + 256, false);
    Json::Value taiko = data::cfg["taiko"];

    rim_key_value[0] = taiko["leftRim"];
    for (Json::Value &v : rim_key_value[0]) {
        chk[v.asInt()] = true;
    }
    centre_key_value[0] = taiko["leftCentre"];
    for (Json::Value &v : centre_key_value[0]) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu!taiko keybinds", "Error reading configs");
            return false;
        }
    }

    std::fill(chk, chk + 256, false);
    rim_key_value[1] = taiko["rightRim"];
    for (Json::Value &v : rim_key_value[1]) {
        chk[v.asInt()] = true;
    }
    centre_key_value[1] = taiko["rightCentre"];
    for (Json::Value &v : centre_key_value[1]) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu!taiko keybinds", "Error reading configs");
            return false;
        }
    }

    // importing sprites
    data::load_texture(gfx.bg_tex, gfx.bg, "img/taiko/bg.png");
    data::load_texture(gfx.up_tex[0], gfx.up[0], "img/taiko/leftup.png");
    data::load_texture(gfx.rim_tex[0], gfx.rim[0], "img/taiko/leftrim.png");
    data::load_texture(gfx.centre_tex[0], gfx.centre[0], "img/taiko/leftcentre.png");
    data::load_texture(gfx.up_tex[1], gfx.up[1], "img/taiko/rightup.png");
    data::load_texture(gfx.rim_tex[1], gfx.rim[1], "img/taiko/rightrim.png");
    data::load_texture(gfx.centre_tex[1], gfx.centre[1], "img/taiko/rightcentre.png");

    return true;
}

void draw() {
    window.draw(gfx.bg);

    // 0 for left side, 1 for right side
    for (int i = 0; i < 2; i++) {
        bool rim_key = false;
        for (Json::Value &v : rim_key_value[i]) {
            if (input::is_pressed(v.asInt())) {
                rim_key = true;
                break;
            }
        }
        if (rim_key) {
            if (!rim_key_state[i]) {
                key_state[i] = 1;
                rim_key_state[i] = true;
            }
        } else {
            rim_key_state[i] = false;
        }

        bool centre_key = false;
        for (Json::Value &v : centre_key_value[i]) {
            if (input::is_pressed(v.asInt())) {
                centre_key = true;
                break;
            }
        }
        if (centre_key) {
            if (!centre_key_state[i]) {
                key_state[i] = 2;
                centre_key_state[i] = true;
            }
        } else {
            centre_key_state[i] = false;
        }

        if (!rim_key_state[i] && !centre_key_state[i]) {
            key_state[i] = 0;
            window.draw(gfx.up[i]);
        }
        if (key_state[i] == 1) {
            if ((clock() - timer_centre_key[i]) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(gfx.rim[i]);
                timer_rim_key[i] = clock();
            } else {
                window.draw(gfx.up[i]);
            }
        } else if (key_state[i] == 2) {
            if ((clock() - timer_rim_key[i]) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(gfx.centre[i]);
                timer_centre_key[i] = clock();
            } else {
                window.draw(gfx.up[i]);
            }
        }
    }
}
}; // namespace taiko
