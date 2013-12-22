CREATE TABLE tweet_topics
(
  id bigint NOT NULL,
  creation_time timestamp,
  user_id bigint,
  retweeted bigint,
  topics text,
  content text,
  CONSTRAINT tweet_topics_pk PRIMARY KEY (id)
);