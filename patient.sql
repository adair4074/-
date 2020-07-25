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

 Date: 25/07/2020 08:59:52
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

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
