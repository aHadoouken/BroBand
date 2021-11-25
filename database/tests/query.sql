
-- psql test -f query.sql

DROP TABLE users;

CREATE TABLE users (
    user_id         SERIAL PRIMARY KEY,
    nickname        varchar(80) UNIQUE NOT NULL,
    profile_avatar  varchar(80) UNIQUE,
    created_at      timestamp
);

INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user1', NULL, now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user2', '/image/1', now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user3', '/image/2', now());
INSERT INTO users(nickname, profile_avatar, created_at) VALUES ('user4', '/image/3', now())
