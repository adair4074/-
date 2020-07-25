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

 Date: 25/07/2020 08:59:34
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

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

SET FOREIGN_KEY_CHECKS = 1;
