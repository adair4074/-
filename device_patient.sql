/*
 Navicat Premium Data Transfer

 Source Server         : connect
 Source Server Type    : MySQL
 Source Server Version : 80013
 Source Host           : localhost:3306
 Source Schema         : medical_monitor5

 Target Server Type    : MySQL
 Target Server Version : 80013
 File Encoding         : 65001

 Date: 25/07/2020 08:59:24
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for device_patient
-- ----------------------------
DROP TABLE IF EXISTS `device_patient`;
CREATE TABLE `device_patient`  (
  `dev_id` int(11) NOT NULL,
  `patient_id` int(11) NOT NULL,
  `time` timestamp(0) NULL DEFAULT CURRENT_TIMESTAMP(0),
  INDEX `fk_patient`(`patient_id`) USING BTREE,
  CONSTRAINT `fk_patient` FOREIGN KEY (`patient_id`) REFERENCES `patient` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_patient
-- ----------------------------
INSERT INTO `device_patient` VALUES (1, 2, '2020-07-24 22:13:31');
INSERT INTO `device_patient` VALUES (2, 3, '2020-07-25 00:21:50');
INSERT INTO `device_patient` VALUES (3, 1, '2020-07-25 00:22:07');

SET FOREIGN_KEY_CHECKS = 1;
