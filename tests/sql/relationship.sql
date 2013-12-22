CREATE TABLE relationship
(
  follower_id bigint NOT NULL,
  followed_id bigint NOT NULL,
  CONSTRAINT relationship_id PRIMARY KEY (follower_id, followed_id)
);