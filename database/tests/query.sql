
-- psql test -f query.sql

DROP TABLE users;
DROP TABLE chats;
DROP TABLE user_chats;

CREATE TABLE users (
    user_id         BIGSERIAL PRIMARY KEY,
    nickname        VARCHAR(80) UNIQUE NOT NULL,
    profile_avatar  VARCHAR(80) UNIQUE,
    created_at      TIMESTAMP
);

INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user1', NULL, now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user2', '/image/1', now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user3', '/image/2', now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user4', '/image/3', now());

CREATE TABLE chats (
    id              BIGSERIAL PRIMARY KEY,
    chat_name       VARCHAR(80) NOT NULL,
    total_messages  BIGINT,
    created_at      TIMESTAMP
);

CREATE TABLE user_chats (
    id          BIGSERIAL PRIMARY KEY,
    user_id     BIGINT,
    chat_id     BIGINT
);

INSERT INTO chats(chat_name, total_messages, created_at) VALUES ('chat1', 10, now());
INSERT INTO user_chats(user_id, chat_id) VALUES (1, 1);
INSERT INTO user_chats(user_id, chat_id) VALUES (2, 1);

INSERT INTO chats(chat_name, total_messages, created_at) VALUES ('chat2', 100, now());
INSERT INTO user_chats(user_id, chat_id) VALUES (1, 2);
INSERT INTO user_chats(user_id, chat_id) VALUES (3, 2);
INSERT INTO user_chats(user_id, chat_id) VALUES (4, 2);

INSERT INTO chats(chat_name, total_messages, created_at) VALUES ('chat3', 10, now());
INSERT INTO user_chats(user_id, chat_id) VALUES (1, 3);
INSERT INTO user_chats(user_id, chat_id) VALUES (4, 3);