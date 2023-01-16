-- function signature (PostgreSQL 10)
CREATE OR REPLACE FUNCTION "testeScheduleSerial" () 
RETURNS integer AS $$
  BEGIN
    SELECT * INTO result
    FROM "Schedule" AS S1, "Schedule" AS S2, "Schedule" AS S3
    WHERE (S1."time" < S2."time" AND S2."time" < S3."time") AND (S1."#t" = S3."#t" AND S1."#t" <> S2."#t");
    
    DO $$
    BEGIN
      IF EXISTS (result) THEN
        RETURN 0;
      ELSE
        RETURN 1;
      END IF;
    END $$;

  END;
$$ LANGUAGE plpgsql;



-- Creating Schedule table
-- Schedule (PostgreSQL 10)
CREATE TABLE "Schedule" (
  "time" integer,
  "#t" integer NOT NULL,
  "op" character NOT NULL,
  "attr" character NOT NULL,
  UNIQUE ("time")
);

--example_01 (PostgreSQL 10)
INSERT INTO "Schedule" ("time", "#t", "op", "attr") VALUES
(1, 1, 'R', 'X'),
(2, 2, 'R', 'X'),
(3, 2, 'W', 'X'),
(4, 1, 'W', 'X'),
(5, 2, 'C', '-'),
(6, 1, 'C', '-');
-- calling function - Expected result 0
SELECT "testeScheduleSerial"() AS resp;

-- example_02 (PostgreSQL 10)
INSERT INTO "Schedule" ("time", "#t", "op", "attr") VALUES
(7, 3, 'R', 'X'),
(8, 3, 'R', 'Y'),
(9, 3, 'W', 'Y'),
(10, 3, 'C', '-'),
(11, 4, 'R', 'X'),
(12, 4, 'C', '-');
-- calling function - Expected result 1
SELECT "testeScheduleSerial"() AS resp;
