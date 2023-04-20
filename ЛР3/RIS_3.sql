CREATE USER PVV IDENTIFIED BY 1111;
GRANT ALL PRIVILEGES TO PVV;

--drop table PVV_T
CREATE TABLE PVV_T
(
  x int primary key
);

SELECT * FROM PVV_T;

--drop database link PPP_DB
CREATE DATABASE LINK PPP_DB
   CONNECT TO PPP
   IDENTIFIED BY ppp
   USING '(DESCRIPTION =
(ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.248.14)(PORT = 1521))
(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = orcl)))';
   
SELECT * FROM XXX@PPP_DB;   
   
--INSERT/INSERT
BEGIN
	INSERT INTO PVV_T VALUES(1);
	INSERT INTO XXX@PPP_DB VALUES(1);
	COMMIT;
END;

SELECT * FROM PVV_T;
SELECT * FROM XXX@PPP_DB;

--INSERT/UPDATE
BEGIN
	INSERT INTO PVV_T VALUES(2);
	UPDATE XXX@PPP_DB set x = 2 where x = 1;
	COMMIT;
END;
SELECT * FROM PVV_T;
SELECT * FROM XXX@PPP_DB;

--UPDATE/INSERT
BEGIN
	UPDATE PVV_T SET X = 3 WHERE X = 1;
	INSERT INTO XXX@PPP_DB VALUES(3);
	COMMIT;
END;
SELECT * FROM PVV_T;
SELECT * FROM XXX@PPP_DB;

--CONSTRAINT ERROR
BEGIN
	INSERT INTO PVV_T VALUES(4);
	INSERT INTO XXX@PPP_DB VALUES(3);
COMMIT;
END;

--WAIT
BEGIN
  SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
  UPDATE PVV_T SET X = 1 WHERE X = 2;
  SYS.DBMS_LOCK.SLEEP(60);
  COMMIT;
END;

