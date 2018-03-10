# Aim of this stored procedure:
# find all packets that: before the CRC_BAD packets and after CRC_OK packets

USE greenbus;

DROP PROCEDURE IF EXISTS curtest;

CREATE PROCEDURE curtest()
  BEGIN
    DECLARE v_payload, v_status, v_preOKpayload, v_needNextOK TEXT;
    DECLARE v_inserted INT;

    DECLARE done INT DEFAULT FALSE;

    DECLARE cur CURSOR FOR SELECT
                             status,
                             payload
                           FROM new_pktlog_aa555a0000000101_20180228t094437z;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN cur;
    CREATE TABLE test (
      payload TEXT
    );
    SET v_preOKpayload = '0000';
    SET v_inserted = 0;
    SET v_needNextOK = 0;

    read_loop: LOOP
      FETCH cur INTO v_status, v_payload;

      IF done THEN LEAVE read_loop; END IF;

      IF (strcmp(substring(v_payload, 2, 3), '763') = 0) THEN
        IF (strcmp(v_status, 'CRC_OK') = 0) THEN
          IF (v_needNextOK = 1) THEN
            INSERT INTO test(payload) VALUES (v_payload);
            SET v_needNextOK = 0;
          END IF;
          SET v_preOKpayload = v_payload;
          SET v_inserted = 0;
        ELSEIF (strcmp(v_status, 'CRC_BAD') = 0) THEN
          SET v_needNextOK = 1;
          IF (v_inserted = 0) THEN
            INSERT INTO test (payload) VALUES (v_preOKpayload);
            SET v_inserted = 1;
          END IF;
        END IF;
      END IF;
    END LOOP;
    CLOSE cur;
  END;

