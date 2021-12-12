
-- psql test -f query.sql

-- DROP TABLE users;
-- DROP TABLE chats;
-- DROP TABLE user_chats;
-- DROP TABLE messages;

-- CREATE TABLE users (
--     user_id         BIGSERIAL PRIMARY KEY,
--     login           VARCHAR(80) UNIQUE NOT NULL,
--     name            VARCHAR(80) NOT NULL,
--     password        VARCHAR(80) NOT NULL,
--     profile_avatar  VARCHAR(80) UNIQUE,
--     created_at      TIMESTAMP
-- );

-- CREATE TABLE chats (
--     id              BIGSERIAL PRIMARY KEY,
--     chat_name       VARCHAR(80) NOT NULL,
--     total_messages  BIGINT,
--     created_at      TIMESTAMP
-- );

-- CREATE TABLE user_chats (
--     id          BIGSERIAL PRIMARY KEY,
--     user_id     BIGINT,
--     chat_id     BIGINT
-- );

-- CREATE TABLE messages (
--     id              BIGSERIAL PRIMARY KEY,
--     sender_id       BIGINT NOT NULL,
--     chat_id         BIGINT NOT NULL,
--     text            VARCHAR(150),
--     attachment      VARCHAR(80),
--     created_at      TIMESTAMP
-- );


TRUNCATE users, chats, user_chats, messages RESTART IDENTITY CASCADE;



INSERT INTO users(login, name, password, profile_avatar, created_at) VALUES ('user1', 'name1', 'password1', NULL, now());
INSERT INTO users(login, name, password, profile_avatar, created_at) VALUES ('user2', 'name2', 'password2', '/image/1', now());
INSERT INTO users(login, name, password, profile_avatar, created_at) VALUES ('user3', 'name3', 'password3', '/image/2', now());
INSERT INTO users(login, name, password, profile_avatar, created_at) VALUES ('user4', 'name4', 'password4', '/image/3', now());



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


INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (1, 1, 'message1 1', '/image/1_1', now());
INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (2, 1, NULL, '/image/2_1', now());
INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (3, 2, 'message1 2', '/image/1_2', now());
INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (4, 3, 'message1 3', '/image/1_3', now());
INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (4, 3, 'message2 3', NULL, now());
INSERT INTO messages(sender_id, chat_id, text, attachment, created_at) VALUES (4, 3, 'message3 3', '/image/2_3', now());

-- SELECT chat_id FROM user_chats WHERE user_id=1;

-- WITH rw as (SELECT *, row_number () over (
--          partition by messages.chat_id
--          order by created_at desc
--        ) rn
-- FROM messages LEFT JOIN user_chats
--  ON (messages.chat_id = user_chats.chat_id)
-- WHERE user_id=1
--             )
--             SELECT * FROM rw
--             WHERE rn = 1;
            
-- UPDATE chats
-- SET total_messages = total_messages + 1
-- WHERE id = 1
-- RETURNING total_messages;

-- INSERT INTO messages(author_id, chat_id, message, num_in_chat, created_at) VALUES (1, 1, 'message1 1', 0, now())
-- returning *;

-- SELECT chat_id, message, max(created_at) FROM messages
-- GROUP BY chat_id;
