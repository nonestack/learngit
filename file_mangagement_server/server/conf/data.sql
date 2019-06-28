CREATE DATABASE IF NOT EXISTS client;

USE client;

DROP TABLE IF EXISTS user_information;
DROP TABLE IF EXISTS file_information;

CREATE TABLE user_information(
id				INT				NOT NULL AUTO_INCREMENT,
name			CHAR(20)		NOT NULL,
passwd			CHAR(50)		NOT NULL,
salt			CHAR(5)			NOT NULL,
PRIMARY KEY (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE file_information(
code			INT				NOT NULL AUTO_INCREMENT,
precode			INT				NOT NULL,
userid			INT				NOT NULL,
filename		CHAR(50)		NOT NULL,
filetype		CHAR(1)			NOT NULL,
linkcode		INT				NOT NULL,
md5				CHAR(50)		NULL,
PRIMARY KEY (code)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;


SELECT 'ok' as 'result:';
