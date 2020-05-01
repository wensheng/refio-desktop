CREATE TABLE ref_libraries(
  id integer not null primary key,
  name text not null default "My Library",
  icodeseq blob not null,
  created text);
-- #
CREATE TABLE ref_collections(
  id integer not null primary key,
  lib_id integer not null default 0,
  parent integer not null default 0,
  name text not null,
  created text);
-- #
CREATE TABLE type_enum(
  type text not null primary key,
  structure text not null default "");
-- #
insert into type_enum(type) values("Webpage");
-- #
insert into type_enum(type) values("Note");
-- #
insert into type_enum(type) values("Article");
-- #
insert into type_enum(type) values("Files");
-- #
insert into type_enum(type) values("Book");
-- #
insert into type_enum(type) values("Book Section");
-- #
CREATE TABLE ref_entries(
  id integer not null primary key,
  collection_id integer not null default 0,
  parent integer not null default 0,
  icode text not null,
  title text not null,
  type text not null references type_enum(type),
  info text,
  created text);
-- #
CREATE TABLE ref_notes(
  id integer not null primary key,
  entry_id integer not null default 0,
  title text,
  body text,
  created text,
  lastmodified text);
-- #
CREATE VIRTUAL TABLE entry_index USING fts5(
  title, info, content='ref_entries', content_rowid='id');
-- #
CREATE TRIGGER entry_ai AFTER INSERT on ref_entries BEGIN
  INSERT INTO entry_index(rowid, title, info)
                  VALUES (new.id, new.title, new.info);
END;
-- #
CREATE TRIGGER entry_ad AFTER DELETE on ref_entries BEGIN
  DELETE FROM entry_index WHERE rowid=old.id;
END;
-- #
CREATE TRIGGER entry_au AFTER UPDATE on ref_entries BEGIN
  DELETE FROM entry_index WHERE rowid=old.id;
  INSERT INTO entry_index(rowid, title, info)
                  VALUES (new.id, new.title, new.info);
END;
