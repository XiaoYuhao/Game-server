/*
 Navicat Premium Data Transfer

 Source Server         : MySQL
 Source Server Type    : MySQL
 Source Server Version : 80012
 Source Host           : localhost:3306
 Source Schema         : user

 Target Server Type    : MySQL
 Target Server Version : 80012
 File Encoding         : 65001

 Date: 23/12/2018 15:29:56
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `db1652195` /*!40100 DEFAULT CHARACTER SET gbk */;

USE `db1652195`;

-- ----------------------------
-- Table structure for USER
-- ----------------------------
DROP TABLE IF EXISTS `USER`;
CREATE TABLE `USER` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `password` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '123456',
  `regtime` datetime(6) DEFAULT NULL,
  `logintime` datetime(6) DEFAULT NULL,
  `score` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of USER
-- ----------------------------
BEGIN;
INSERT INTO `USER` VALUES (1, 'user', 'e10adc3949ba59abbe56e057f20f883e', NULL, '2019-5-2 15:26:52.000000',0);
INSERT INTO `USER` VALUES (2, 'xiao', 'e10adc3949ba59abbe56e057f20f883e', NULL, NULL,0);
INSERT INTO `USER` VALUES (3, 'hello', 'e10adc3949ba59abbe56e057f20f883e', NULL, NULL,0);
INSERT INTO `USER` VALUES (4, 'test', 'e10adc3949ba59abbe56e057f20f883e', NULL, '2019-5-2 14:45:31.000000',0);
COMMIT;

SET FOREIGN_KEY_CHECKS = 1;
