#include "player.h"
Player::Player()
    : m_Speed(START_SPEED),
    m_Health(START_HEALTH),
    m_MaxHealth(START_HEALTH),
    m_Texture(),
    m_Sprite()
{
    // Associate a texture with the sprite
    // !!Watch this space!!
    m_Texture.LoadFromFile("grapchis/player.png");
    m_Sprite.setTexture(m_Texture);

    // Set the origin of the sprite to the center,
    // for smooth rotation
    m_Sprite.setOrigin({25, 25});
}
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
    // Place the player in the middle of the arena
    m_Position.x = arena.width / 2;
    m_Position.y = arena.height / 2;
    // Copy the details of the arena
    // To the player's m_Arena
    m_Arena.left = arena.left;
    m_Arena.width = arena.width;
    m_Arena.top = arena.top;
    m_Arena.height = arena.height;
    // Remember how bit the tiles are in this arena
    m_TileSize = tileSize;
    // Store the resolution for future use
    m_Resolution.x = resolution.x;
    m_Resolution.y = resolution.y;
}
void Player::resetPlayerStats()
{
    m_Speed = START_SPEED;
    m_Health = START_HEALTH;
    m_MaxHealth = START_HEALTH;
}
Time Player::getLastHitTime()
{
    return m_LastHit;
}
bool Player::hit(Time timeHit)
{
    if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
    {
        m_LastHit = timeHit;
        m_Health -= 10;
        return true;
    }
    else 
    {
        return false;
    }
}
FloactRect Player::getPosition()
{
    return m_Sprite.getGloblBounds;
}
Vector2f Pplayer::getCenter()
{
    return m_Position;
}
float Player::getRotation()
{
    return m_Sprite.getRotation();
}
Sprite Player::getSprite()
{
    return m_Sprite;
}
int Player::getHealth()
{
    return m_Health;
}
void Player::moveLdeft()
{
    m_LeftPressed = true;
}
void Player::moveRIght()
{
    m_RightPressed = true;
}
void Player::moveUp()
{
    mUpPressed = true;
}
void Player::moveDown()
{
    m_DownPressed = true;
}
void Player::stopLeft()
{
    m_RightPressed = false;
}
void Player::stopRight()
{
    m_RightPressed = false;
}
void Player::stopUp()
{
    m_UpPressed = false;
}
void Player::stopDown()
{
    m_DownPressed = false;
}
