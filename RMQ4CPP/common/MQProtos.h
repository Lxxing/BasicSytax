/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* ��ӭ����ָ�� xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __MQPROTOS_H__
#define __MQPROTOS_H__

namespace lxmq {

enum MQRequestCode {
  // send msg to Broker
  SEND_MESSAGE = 10,
  // subscribe msg from Broker
  PULL_MESSAGE = 11,
  // query msg from Broker
  QUERY_MESSAGE = 12,
  // query Broker Offset
  QUERY_BROKER_OFFSET = 13,
  // query Consumer Offset from broker
  QUERY_CONSUMER_OFFSET = 14,
  // update Consumer Offset to broker
  UPDATE_CONSUMER_OFFSET = 15,
  // create or update Topic to broker
  UPDATE_AND_CREATE_TOPIC = 17,
  // get all topic config info from broker
  GET_ALL_TOPIC_CONFIG = 21,
  // git all topic list from broker
  GET_TOPIC_CONFIG_LIST = 22,
  // get topic name list from broker
  GET_TOPIC_NAME_LIST = 23,
  UPDATE_BROKER_CONFIG = 25,
  GET_BROKER_CONFIG = 26,
  TRIGGER_DELETE_FILES = 27,
  GET_BROKER_RUNTIME_INFO = 28,
  SEARCH_OFFSET_BY_TIMESTAMP = 29,
  GET_MAX_OFFSET = 30,
  GET_MIN_OFFSET = 31,
  GET_EARLIEST_MSG_STORETIME = 32,
  VIEW_MESSAGE_BY_ID = 33,
  // send heartbeat to broker, and register itself
  HEART_BEAT = 34,
  // unregister client to broker
  UNREGISTER_CLIENT = 35,
  // send back consume fail msg to broker
  CONSUMER_SEND_MSG_BACK = 36,
  // Commit Or Rollback transaction
  END_TRANSACTION = 37,
  // get consumer list by group from broker
  GET_CONSUMER_LIST_BY_GROUP = 38,

  CHECK_TRANSACTION_STATE = 39,
  // broker send notify to consumer when consumer lists changes
  NOTIFY_CONSUMER_IDS_CHANGED = 40,
  // lock mq before orderly consume
  LOCK_BATCH_MQ = 41,
  // unlock mq after orderly consume
  UNLOCK_BATCH_MQ = 42,
  GET_ALL_CONSUMER_OFFSET = 43,
  GET_ALL_DELAY_OFFSET = 45,
  PUT_KV_CONFIG = 100,
  GET_KV_CONFIG = 101,
  DELETE_KV_CONFIG = 102,
  REGISTER_BROKER = 103,
  UNREGISTER_BROKER = 104,
  GET_ROUTEINTO_BY_TOPIC = 105,
  GET_BROKER_CLUSTER_INFO = 106,
  UPDATE_AND_CREATE_SUBSCRIPTIONGROUP = 200,
  GET_ALL_SUBSCRIPTIONGROUP_CONFIG = 201,
  GET_TOPIC_STATS_INFO = 202,
  GET_CONSUMER_CONNECTION_LIST = 203,
  GET_PRODUCER_CONNECTION_LIST = 204,
  WIPE_WRITE_PERM_OF_BROKER = 205,

  GET_ALL_TOPIC_LIST_FROM_NAMESERVER = 206,
  DELETE_SUBSCRIPTIONGROUP = 207,
  GET_CONSUME_STATS = 208,
  SUSPEND_CONSUMER = 209,
  RESUME_CONSUMER = 210,
  RESET_CONSUMER_OFFSET_IN_CONSUMER = 211,
  RESET_CONSUMER_OFFSET_IN_BROKER = 212,
  ADJUST_CONSUMER_THREAD_POOL = 213,
  WHO_CONSUME_THE_MESSAGE = 214,

  DELETE_TOPIC_IN_BROKER = 215,
  DELETE_TOPIC_IN_NAMESRV = 216,

  GET_KV_CONFIG_BY_VALUE = 217,

  DELETE_KV_CONFIG_BY_VALUE = 218,

  GET_KVLIST_BY_NAMESPACE = 219,

  RESET_CONSUMER_CLIENT_OFFSET = 220,

  GET_CONSUMER_STATUS_FROM_CLIENT = 221,

  INVOKE_BROKER_TO_RESET_OFFSET = 222,

  INVOKE_BROKER_TO_GET_CONSUMER_STATUS = 223,

  QUERY_TOPIC_CONSUME_BY_WHO = 300,

  GET_TOPICS_BY_CLUSTER = 224,

  REGISTER_FILTER_SERVER = 301,

  REGISTER_MESSAGE_FILTER_CLASS = 302,

  QUERY_CONSUME_TIME_SPAN = 303,

  GET_SYSTEM_TOPIC_LIST_FROM_NS = 304,
  GET_SYSTEM_TOPIC_LIST_FROM_BROKER = 305,

  CLEAN_EXPIRED_CONSUMEQUEUE = 306,

  GET_CONSUMER_RUNNING_INFO = 307,

  QUERY_CORRECTION_OFFSET = 308,

  CONSUME_MESSAGE_DIRECTLY = 309,

  SEND_MESSAGE_V2 = 310,

  GET_UNIT_TOPIC_LIST = 311,
  GET_HAS_UNIT_SUB_TOPIC_LIST = 312,
  GET_HAS_UNIT_SUB_UNUNIT_TOPIC_LIST = 313,

  CLONE_GROUP_OFFSET = 314,

  VIEW_BROKER_STATS_DATA = 315,
  
  CLEAN_UNUSED_TOPIC = 316,

  GET_BROKER_CONSUME_STATS = 317,
    
  UPDATE_NAMESRV_CONFIG = 318,//update the config of name server
  
  GET_NAMESRV_CONFIG = 319,//get config from name server

  QUERY_DATA_VERSION = 322
};

//<!***************************************************************************
enum MQResponseCode {
  // rcv success response from broker
  SUCCESS_VALUE = 0,
  // rcv exception from broker
  SYSTEM_ERROR = 1,
  // rcv symtem busy from broker
  SYSTEM_BUSY = 2,
  // broker don't support the request code
  REQUEST_CODE_NOT_SUPPORTED = 3,
  // broker flush disk timeout error
  FLUSH_DISK_TIMEOUT = 10,
  // broker sync double write, slave broker not available
  SLAVE_NOT_AVAILABLE = 11,
  // broker sync double write, slave broker flush msg timeout
  FLUSH_SLAVE_TIMEOUT = 12,
  // broker rcv illegal mesage
  MESSAGE_ILLEGAL = 13,
  // service not available due to broker or namesrv in shutdown status
  SERVICE_NOT_AVAILABLE = 14,
  // this version is not supported on broker or namesrv
  VERSION_NOT_SUPPORTED = 15,
  // broker or Namesrv has no permission to do this operation
  NO_PERMISSION = 16,
  // topic is not exist on broker
  TOPIC_NOT_EXIST = 17,
  // broker already created this topic
  TOPIC_EXIST_ALREADY = 18,
  // pulled msg was not found
  PULL_NOT_FOUND = 19,
  // retry later
  PULL_RETRY_IMMEDIATELY = 20,
  // pull msg with wrong offset
  PULL_OFFSET_MOVED = 21,
  // could not find the query msg
  QUERY_NOT_FOUND = 22,

  SUBSCRIPTION_PARSE_FAILED = 23,
  SUBSCRIPTION_NOT_EXIST = 24,
  SUBSCRIPTION_NOT_LATEST = 25,
  SUBSCRIPTION_GROUP_NOT_EXIST = 26,

  TRANSACTION_SHOULD_COMMIT = 200,
  TRANSACTION_SHOULD_ROLLBACK = 201,
  TRANSACTION_STATE_UNKNOW = 202,
  TRANSACTION_STATE_GROUP_WRONG = 203,

  CONSUMER_NOT_ONLINE = 206,
  CONSUME_MSG_TIMEOUT = 207
};

}  //end lxmq;

#endif
