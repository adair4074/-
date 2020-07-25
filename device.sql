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

 Date: 25/07/2020 08:59:10
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for device
-- ----------------------------
DROP TABLE IF EXISTS `device`;
CREATE TABLE `device`  (
  `dev_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `serial` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `refresh` timestamp(0) NOT NULL,
  PRIMARY KEY (`dev_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device
-- ----------------------------
INSERT INTO `device` VALUES (1, 'DEV-002', '2020-07-25 00:22:26');
INSERT INTO `device` VALUES (2, 'DEV_003', '2020-07-24 20:54:02');
INSERT INTO `device` VALUES (3, 'DEV_004', '2020-07-24 20:54:05');

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

-- ----------------------------
-- Table structure for doctor
-- ----------------------------
DROP TABLE IF EXISTS `doctor`;
CREATE TABLE `doctor`  (
  `uid` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `name` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `mobile` bigint(11) UNSIGNED NULL DEFAULT NULL,
  PRIMARY KEY (`uid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of doctor
-- ----------------------------
INSERT INTO `doctor` VALUES ('doctor1', '张三丰', 13123456789);
INSERT INTO `doctor` VALUES ('doctor2', '张无忌', 13987654322);

-- ----------------------------
-- Table structure for ecg_sample
-- ----------------------------
DROP TABLE IF EXISTS `ecg_sample`;
CREATE TABLE `ecg_sample`  (
  `value` varbinary(1000) NOT NULL,
  `time` timestamp(0) NOT NULL,
  `dev_id` int(10) UNSIGNED NULL DEFAULT NULL,
  INDEX `fk_sample_dev`(`dev_id`) USING BTREE,
  CONSTRAINT `fk_dev_id` FOREIGN KEY (`dev_id`) REFERENCES `device` (`dev_id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_sample_dev` FOREIGN KEY (`dev_id`) REFERENCES `device` (`dev_id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of ecg_sample
-- ----------------------------
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:09:04', NULL);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:26:41', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:26:46', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:27:58', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:28:03', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:39:35', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 20:39:41', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 21:02:58', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 21:16:56', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 21:18:09', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 21:24:12', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 21:55:17', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 22:03:26', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 22:04:23', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 22:10:42', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 22:13:39', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-24 23:17:50', 1);
INSERT INTO `ecg_sample` VALUES (0xF807F907FA07FB07FC07FD07FE07FF0700080108, '2020-07-25 00:22:26', 1);

-- ----------------------------
-- Table structure for patient
-- ----------------------------
DROP TABLE IF EXISTS `patient`;
CREATE TABLE `patient`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` char(5) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `sex` char(1) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of patient
-- ----------------------------
INSERT INTO `patient` VALUES (1, '张三', '男');
INSERT INTO `patient` VALUES (2, '张四', '男');
INSERT INTO `patient` VALUES (3, '王五', '男');

SET FOREIGN_KEY_CHECKS = 1;
