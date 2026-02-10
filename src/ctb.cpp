#include "header.hpp"

namespace ctb {
Json::Value left_key_value, right_key_value, dash_key_value;
struct Graphics{
    sf::Texture bgTex, midTex, leftTex, rightTex, dashTex, upTex;
    sf::Sprite bg, mid, left, right, dash, up;

    Graphics() : bg(bgTex), mid(midTex), left(leftTex), right(rightTex), dash(dashTex), up(upTex){}
};

Graphics gfx;

int key_state = 0;
bool left_key_state = false;
bool right_key_state = false;
double timer_left_key = -1;
double timer_right_key = -1;

bool init() {
    // getting configs
    Json::Value ctb = data::cfg["catch"];

    bool chk[256];
    std::fill(chk, chk + 256, false);
    left_key_value = ctb["left"];
    for (Json::Value &v : left_key_value) {
        chk[v.asInt()] = true;
    }
    right_key_value = ctb["right"];
    for (Json::Value &v : right_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu!catch keybinds", "Error reading configs");
            return false;
        }
    }
    dash_key_value = ctb["dash"];

    // importing sprites
    data::load_texture(gfx.bgTex, gfx.bg, "img/catch/bg.png");
    data::load_texture(gfx.midTex, gfx.mid, "img/catch/mid.png");
    data::load_texture(gfx.leftTex, gfx.left, "img/catch/left.png");
    data::load_texture(gfx.rightTex, gfx.right, "img/catch/right.png");
    data::load_texture(gfx.dashTex, gfx.dash, "img/catch/dash.png");
    data::load_texture(gfx.upTex, gfx.up, "img/catch/up.png");

    return true;
}

void draw() {
    window.draw(gfx.bg);
    
    // drawing left-right keypresses
    bool left_key = false;
    for (Json::Value &v : left_key_value) {
        if (input::is_pressed(v.asInt())) {
            left_key = true;
            break;
        }
    }
    if (left_key) {
        if (!left_key_state) {
            key_state = 1;
            left_key_state = true;
        }
    } else {
        left_key_state = false;
    }

    bool right_key = false;
    for (Json::Value &v : right_key_value) {
        if (input::is_pressed(v.asInt())) {
            right_key = true;
            break;
        }
    }
    if (right_key) {
        if (!right_key_state) {
            key_state = 2;
            right_key_state = true;
        }
    } else {
        right_key_state = false;
    }

    if (!left_key_state && !right_key_state) {
        key_state = 0;
        window.draw(gfx.mid);
    }
    if (key_state == 1) {
        if ((clock() - timer_right_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(gfx.left);
            timer_left_key = clock();
        } else {
            window.draw(gfx.mid);
        }
    } else if (key_state == 2) {
        if ((clock() - timer_left_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(gfx.right);
            timer_right_key = clock();
        } else {
            window.draw(gfx.mid);
        }
    }

    bool is_dash = false;
    for (Json::Value &v : dash_key_value) {
        if (input::is_pressed(v.asInt())) {
            window.draw(gfx.dash);
            is_dash = true;
            break;
        }
    }
    if (!is_dash) {
        window.draw(gfx.up);
    }
}
}; // namespace ctb
