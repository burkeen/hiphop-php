/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/
// Get SIZE_MAX definition.  Do this before including any other files, to make
// sure that this is the first place that stdint.h is included.
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include <runtime/base/runtime_option.h>
#include <runtime/base/type_conversions.h>
#include <runtime/base/builtin_functions.h>
#include <runtime/base/shared/shared_store_base.h>
#include <runtime/base/server/access_log.h>
#include <runtime/base/memory/leak_detectable.h>
#include <runtime/base/util/extended_logger.h>
#include <runtime/base/util/simple_counter.h>
#include <util/util.h>
#include <util/network.h>
#include <util/logger.h>
#include <util/stack_trace.h>
#include <util/process.h>
#include <util/file_cache.h>
#include <util/hardware_counter.h>
#include <runtime/base/preg.h>
#include <util/parser/scanner.h>
#include <runtime/base/server/access_log.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

bool RuntimeOption::Loaded = false;

const char *RuntimeOption::ExecutionMode = "";
std::string RuntimeOption::BuildId;
std::string RuntimeOption::PidFile = "www.pid";

std::string RuntimeOption::LogFile;
std::string RuntimeOption::LogFileSymLink;
std::string RuntimeOption::LogAggregatorFile;
std::string RuntimeOption::LogAggregatorDatabase;
int RuntimeOption::LogAggregatorSleepSeconds = 10;
bool RuntimeOption::AlwaysEscapeLog = false;
bool RuntimeOption::AlwaysLogUnhandledExceptions = true;
bool RuntimeOption::InjectedStackTrace = true;
int RuntimeOption::InjectedStackTraceLimit = -1;
bool RuntimeOption::NoSilencer = false;
bool RuntimeOption::EnableApplicationLog = true;
bool RuntimeOption::CallUserHandlerOnFatals = true;
bool RuntimeOption::ThrowExceptionOnBadMethodCall = true;
int RuntimeOption::RuntimeErrorReportingLevel = ErrorConstants::HPHP_ALL;

std::string RuntimeOption::ServerUser;

int RuntimeOption::MaxLoopCount = 0;
int RuntimeOption::MaxSerializedStringSize = 64 * 1024 * 1024; // 64MB
bool RuntimeOption::NoInfiniteRecursionDetection = false;
bool RuntimeOption::ThrowBadTypeExceptions = false;
bool RuntimeOption::ThrowTooManyArguments = false;
bool RuntimeOption::WarnTooManyArguments = false;
bool RuntimeOption::ThrowMissingArguments = false;
bool RuntimeOption::ThrowInvalidArguments = false;
bool RuntimeOption::EnableHipHopErrors = true;
bool RuntimeOption::AssertActive = false;
bool RuntimeOption::AssertWarning = false;
int RuntimeOption::NoticeFrequency = 1;
int RuntimeOption::WarningFrequency = 1;
int64 RuntimeOption::SerializationSizeLimit = StringData::MaxSize;
int64 RuntimeOption::StringOffsetLimit = 10 * 1024 * 1024; // 10MB

std::string RuntimeOption::AccessLogDefaultFormat;
std::vector<AccessLogFileData> RuntimeOption::AccessLogs;

std::string RuntimeOption::AdminLogFormat;
std::string RuntimeOption::AdminLogFile;
std::string RuntimeOption::AdminLogSymLink;


std::string RuntimeOption::Tier;
std::string RuntimeOption::Host;
std::string RuntimeOption::DefaultServerNameSuffix;
std::string RuntimeOption::ServerIP;
std::string RuntimeOption::ServerPrimaryIP;
int RuntimeOption::ServerPort;
int RuntimeOption::ServerPortFd = -1;
int RuntimeOption::ServerBacklog = 128;
int RuntimeOption::ServerConnectionLimit = 0;
int RuntimeOption::ServerThreadCount = 50;
bool RuntimeOption::ServerThreadRoundRobin = false;
int RuntimeOption::ServerThreadDropCacheTimeoutSeconds = 0;
bool RuntimeOption::ServerThreadJobLIFO = false;
bool RuntimeOption::ServerThreadDropStack = false;
bool RuntimeOption::ServerHttpSafeMode = false;
bool RuntimeOption::ServerStatCache = true;
int RuntimeOption::PageletServerThreadCount = 0;
bool RuntimeOption::PageletServerThreadRoundRobin = false;
int RuntimeOption::PageletServerThreadDropCacheTimeoutSeconds = 0;
int RuntimeOption::PageletServerQueueLimit = 0;
bool RuntimeOption::PageletServerThreadDropStack = false;
int RuntimeOption::FiberCount = 1;
int RuntimeOption::RequestTimeoutSeconds = 0;
size_t RuntimeOption::ServerMemoryHeadRoom = 0;
int64 RuntimeOption::RequestMemoryMaxBytes = INT64_MAX;
int64 RuntimeOption::ImageMemoryMaxBytes = 0;
int RuntimeOption::ResponseQueueCount;
int RuntimeOption::ServerGracefulShutdownWait;
bool RuntimeOption::ServerHarshShutdown = true;
bool RuntimeOption::ServerEvilShutdown = true;
int RuntimeOption::ServerDanglingWait;
int RuntimeOption::ServerShutdownListenWait = 0;
int RuntimeOption::ServerShutdownListenNoWork = -1;
int RuntimeOption::GzipCompressionLevel = 3;
std::string RuntimeOption::ForceCompressionURL;
std::string RuntimeOption::ForceCompressionCookie;
std::string RuntimeOption::ForceCompressionParam;
bool RuntimeOption::EnableMagicQuotesGpc = false;
bool RuntimeOption::EnableKeepAlive = true;
bool RuntimeOption::ExposeHPHP = true;
bool RuntimeOption::ExposeXFBServer = false;
bool RuntimeOption::ExposeXFBDebug = false;
std::string RuntimeOption::XFBDebugSSLKey;
int RuntimeOption::ConnectionTimeoutSeconds = -1;
bool RuntimeOption::EnableOutputBuffering = false;
std::string RuntimeOption::OutputHandler;
bool RuntimeOption::ImplicitFlush = false;
bool RuntimeOption::EnableEarlyFlush = true;
bool RuntimeOption::ForceChunkedEncoding = false;
int64 RuntimeOption::MaxPostSize;
bool RuntimeOption::AlwaysPopulateRawPostData = true;
int64 RuntimeOption::UploadMaxFileSize;
std::string RuntimeOption::UploadTmpDir;
bool RuntimeOption::EnableFileUploads;
bool RuntimeOption::EnableUploadProgress;
int RuntimeOption::Rfc1867Freq;
std::string RuntimeOption::Rfc1867Prefix;
std::string RuntimeOption::Rfc1867Name;
bool RuntimeOption::LibEventSyncSend = true;
bool RuntimeOption::ExpiresActive = true;
int RuntimeOption::ExpiresDefault = 2592000;
std::string RuntimeOption::DefaultCharsetName = "utf-8";
bool RuntimeOption::ForceServerNameToHeader = false;
bool RuntimeOption::EnableCufAsync = false;

int RuntimeOption::RequestBodyReadLimit = -1;

bool RuntimeOption::EnableSSL = false;
int RuntimeOption::SSLPort = 443;
int RuntimeOption::SSLPortFd = -1;
std::string RuntimeOption::SSLCertificateFile;
std::string RuntimeOption::SSLCertificateKeyFile;
std::string RuntimeOption::SSLCertificateDir;

VirtualHostPtrVec RuntimeOption::VirtualHosts;
IpBlockMapPtr RuntimeOption::IpBlocks;
SatelliteServerInfoPtrVec RuntimeOption::SatelliteServerInfos;

int RuntimeOption::XboxServerThreadCount = 10;
int RuntimeOption::XboxServerMaxQueueLength = INT_MAX;
int RuntimeOption::XboxServerPort = 0;
int RuntimeOption::XboxDefaultLocalTimeoutMilliSeconds = 500;
int RuntimeOption::XboxDefaultRemoteTimeoutSeconds = 5;
int RuntimeOption::XboxServerInfoMaxRequest = 500;
int RuntimeOption::XboxServerInfoDuration = 120;
std::string RuntimeOption::XboxServerInfoWarmupDoc;
std::string RuntimeOption::XboxServerInfoReqInitFunc;
std::string RuntimeOption::XboxServerInfoReqInitDoc;
bool RuntimeOption::XboxServerInfoAlwaysReset = false;
bool RuntimeOption::XboxServerLogInfo = false;
std::string RuntimeOption::XboxProcessMessageFunc = "xbox_process_message";
std::string RuntimeOption::XboxPassword;
std::set<std::string> RuntimeOption::XboxPasswords;

std::string RuntimeOption::SourceRoot = Process::GetCurrentDirectory() + '/';
std::vector<std::string> RuntimeOption::IncludeSearchPaths;
std::string RuntimeOption::FileCache;
std::string RuntimeOption::DefaultDocument;
std::string RuntimeOption::ErrorDocument404;
bool RuntimeOption::ForbiddenAs404 = false;
std::string RuntimeOption::ErrorDocument500;
std::string RuntimeOption::FatalErrorMessage;
std::string RuntimeOption::FontPath;
bool RuntimeOption::EnableStaticContentCache = true;
bool RuntimeOption::EnableStaticContentFromDisk = true;
bool RuntimeOption::EnableOnDemandUncompress = true;
bool RuntimeOption::EnableStaticContentMMap = true;

std::string RuntimeOption::RTTIDirectory;
bool RuntimeOption::EnableCliRTTI = false;
bool RuntimeOption::Utf8izeReplace = true;

std::string RuntimeOption::StartupDocument;
std::string RuntimeOption::WarmupDocument;
std::string RuntimeOption::RequestInitFunction;
std::string RuntimeOption::RequestInitDocument;
std::vector<std::string> RuntimeOption::ThreadDocuments;
std::vector<std::string> RuntimeOption::ThreadLoopDocuments;

bool RuntimeOption::SafeFileAccess = false;
std::vector<std::string> RuntimeOption::AllowedDirectories;
std::set<std::string> RuntimeOption::AllowedFiles;
hphp_string_imap<std::string> RuntimeOption::StaticFileExtensions;
std::set<std::string> RuntimeOption::ForbiddenFileExtensions;
std::set<std::string> RuntimeOption::StaticFileGenerators;
FilesMatchPtrVec RuntimeOption::FilesMatches;

bool RuntimeOption::WhitelistExec = false;
bool RuntimeOption::WhitelistExecWarningOnly = false;
std::vector<std::string> RuntimeOption::AllowedExecCmds;

std::string RuntimeOption::TakeoverFilename;
int RuntimeOption::AdminServerPort;
int RuntimeOption::AdminThreadCount = 1;
std::string RuntimeOption::AdminPassword;
std::set<std::string> RuntimeOption::AdminPasswords;

std::string RuntimeOption::ProxyOrigin;
int RuntimeOption::ProxyRetry = 3;
bool RuntimeOption::UseServeURLs;
std::set<std::string> RuntimeOption::ServeURLs;
bool RuntimeOption::UseProxyURLs;
int RuntimeOption::ProxyPercentage = 0;
std::set<std::string> RuntimeOption::ProxyURLs;
std::vector<std::string> RuntimeOption::ProxyPatterns;
bool RuntimeOption::AlwaysUseRelativePath = false;

bool RuntimeOption::MySQLReadOnly = false;
bool RuntimeOption::MySQLLocalize = false;
int RuntimeOption::MySQLConnectTimeout = 1000;
int RuntimeOption::MySQLReadTimeout = 1000;
int RuntimeOption::MySQLWaitTimeout = -1;
int RuntimeOption::MySQLSlowQueryThreshold = 1000; // ms
bool RuntimeOption::MySQLKillOnTimeout = false;
int RuntimeOption::MySQLMaxRetryOpenOnFail = 1;
int RuntimeOption::MySQLMaxRetryQueryOnFail = 1;

int RuntimeOption::HttpDefaultTimeout = 30;
int RuntimeOption::HttpSlowQueryThreshold = 5000; // ms

bool RuntimeOption::TranslateLeakStackTrace = false;
bool RuntimeOption::NativeStackTrace = false;
bool RuntimeOption::FullBacktrace = false;
bool RuntimeOption::ServerStackTrace = false;
bool RuntimeOption::ServerErrorMessage = false;
bool RuntimeOption::TranslateSource = false;
bool RuntimeOption::RecordInput = false;
bool RuntimeOption::ClearInputOnSuccess = true;
std::string RuntimeOption::ProfilerOutputDir;
std::string RuntimeOption::CoreDumpEmail;
bool RuntimeOption::CoreDumpReport = true;
bool RuntimeOption::LocalMemcache = false;
bool RuntimeOption::MemcacheReadOnly = false;

bool RuntimeOption::EnableStats = false;
bool RuntimeOption::EnableWebStats = false;
bool RuntimeOption::EnableMemoryStats = false;
bool RuntimeOption::EnableMallocStats = false;
bool RuntimeOption::EnableAPCStats = false;
bool RuntimeOption::EnableAPCKeyStats = false;
bool RuntimeOption::EnableMemcacheStats = false;
bool RuntimeOption::EnableMemcacheKeyStats = false;
bool RuntimeOption::EnableSQLStats = false;
bool RuntimeOption::EnableSQLTableStats = false;
bool RuntimeOption::EnableNetworkIOStatus = false;
std::string RuntimeOption::StatsXSL;
std::string RuntimeOption::StatsXSLProxy;
int RuntimeOption::StatsSlotDuration = 10 * 60; // 10 minutes
int RuntimeOption::StatsMaxSlot = 12 * 6; // 12 hours

bool RuntimeOption::EnableAPCSizeStats = false;
bool RuntimeOption::EnableAPCSizeGroup = false;
std::vector<std::string> RuntimeOption::APCSizeSpecialPrefix;
std::vector<std::string> RuntimeOption::APCSizePrefixReplace;
std::vector<std::string> RuntimeOption::APCSizeSpecialMiddle;
std::vector<std::string> RuntimeOption::APCSizeMiddleReplace;
std::vector<std::string> RuntimeOption::APCSizeSkipPrefix;
bool RuntimeOption::EnableAPCSizeDetail = false;
bool RuntimeOption::EnableAPCFetchStats = false;
bool RuntimeOption::APCSizeCountPrime = false;

int64 RuntimeOption::MaxRSS = 0;
int64 RuntimeOption::MaxRSSPollingCycle = 0;
int64 RuntimeOption::DropCacheCycle = 0;
int64 RuntimeOption::MaxSQLRowCount = 10000;
int64 RuntimeOption::MaxMemcacheKeyCount = 0;
int RuntimeOption::SocketDefaultTimeout = 5;
bool RuntimeOption::LockCodeMemory = false;
bool RuntimeOption::EnableMemoryManager = true;
bool RuntimeOption::CheckMemory = false;
int RuntimeOption::MaxArrayChain = INT_MAX;
bool RuntimeOption::UseHphpArray = hhvm;
bool RuntimeOption::UseSmallArray = false;
bool RuntimeOption::UseVectorArray = true;
bool RuntimeOption::StrictCollections = false;
bool RuntimeOption::WarnOnCollectionToArray = false;
bool RuntimeOption::UseDirectCopy = false;
bool RuntimeOption::EnableApc = true;
bool RuntimeOption::EnableConstLoad = false;
bool RuntimeOption::ForceConstLoadToAPC = true;
std::string RuntimeOption::ApcPrimeLibrary;
int RuntimeOption::ApcLoadThread = 1;
std::set<std::string> RuntimeOption::ApcCompletionKeys;
RuntimeOption::ApcTableTypes RuntimeOption::ApcTableType = ApcConcurrentTable;
bool RuntimeOption::EnableApcSerialize = true;
time_t RuntimeOption::ApcKeyMaturityThreshold = 20;
size_t RuntimeOption::ApcMaximumCapacity = 0;
int RuntimeOption::ApcKeyFrequencyUpdatePeriod = 1000;
bool RuntimeOption::ApcExpireOnSets = false;
int RuntimeOption::ApcPurgeFrequency = 4096;
int RuntimeOption::ApcPurgeRate = -1;
bool RuntimeOption::ApcAllowObj = false;
int RuntimeOption::ApcTTLLimit = -1;
bool RuntimeOption::ApcUseFileStorage = false;
int64 RuntimeOption::ApcFileStorageChunkSize = 1LL << 29;
int64 RuntimeOption::ApcFileStorageMaxSize = 1LL << 32;
std::string RuntimeOption::ApcFileStoragePrefix;
int RuntimeOption::ApcFileStorageAdviseOutPeriod = 1800;
std::string RuntimeOption::ApcFileStorageFlagKey;
bool RuntimeOption::ApcConcurrentTableLockFree = false;
bool RuntimeOption::ApcFileStorageKeepFileLinked = false;
std::vector<std::string> RuntimeOption::ApcNoTTLPrefix;

bool RuntimeOption::EnableDnsCache = false;
int RuntimeOption::DnsCacheTTL = 10 * 60; // 10 minutes
time_t RuntimeOption::DnsCacheKeyMaturityThreshold = 20;
size_t RuntimeOption::DnsCacheMaximumCapacity = 0;
int RuntimeOption::DnsCacheKeyFrequencyUpdatePeriod = 1000;

std::map<std::string, std::string> RuntimeOption::ServerVariables;
std::map<std::string, std::string> RuntimeOption::EnvVariables;

std::string RuntimeOption::LightProcessFilePrefix;
int RuntimeOption::LightProcessCount;

bool RuntimeOption::EnableHipHopSyntax = false;
bool RuntimeOption::EnableHipHopExperimentalSyntax = false;
bool RuntimeOption::EnableShortTags = true;
bool RuntimeOption::EnableAspTags = false;
bool RuntimeOption::EnableXHP = true;
bool RuntimeOption::EnableObjDestructCall = false;
bool RuntimeOption::EnableEmitSwitch = true;
bool RuntimeOption::CheckSymLink = false;
bool RuntimeOption::NativeXHP = true;
int RuntimeOption::ScannerType = 0;
int RuntimeOption::MaxUserFunctionId = (2 * 65536);
bool RuntimeOption::EnableFinallyStatement = false;

#ifdef TAINTED
bool RuntimeOption::EnableTaintWarnings = false;
int RuntimeOption::TaintTraceMaxStrlen = 127;
#endif

// TODO: Task #1154042: These runtime options are no longer used, remove them
bool RuntimeOption::EnableEvalOptimization = true;
int RuntimeOption::EvalScalarValueExprLimit = 64;
bool RuntimeOption::SandboxCheckMd5 = false;
bool RuntimeOption::EnableStrict = false;
int RuntimeOption::StrictLevel = 1;
bool RuntimeOption::StrictFatal = false;

const uint64_t kEvalVMStackElmsDefault =
#ifdef VALGRIND
 0x800
#else
 0x4000
#endif
 ;
const uint32_t kEvalVMInitialGlobalTableSizeDefault = 512;
uint64 RuntimeOption::EvalVMStackElms = kEvalVMStackElmsDefault;
uint32_t RuntimeOption::EvalVMInitialGlobalTableSize =
  kEvalVMInitialGlobalTableSizeDefault;
bool RuntimeOption::EvalJit = false;
bool RuntimeOption::EvalAllowHhas = false;
std::string RuntimeOption::EvalJitProfilePath = "/tmp/hhvm-profile";
int RuntimeOption::EvalJitStressTypePredPercent = 0;
static const int kDefaultWarmupRequests = debug ? 1 : 11;
uint32 RuntimeOption::EvalJitWarmupRequests = kDefaultWarmupRequests;
bool RuntimeOption::EvalJitProfileRecord = false;
bool RuntimeOption::EvalJitNoGdb = true;
bool RuntimeOption::EvalProfileBC = false;
bool RuntimeOption::EvalProfileHWEnable = true;
std::string RuntimeOption::EvalProfileHWEvents = "";
#define JIT_TRAMPOLINES_DEFAULT true
bool RuntimeOption::EvalJitTrampolines = JIT_TRAMPOLINES_DEFAULT;
uint32 RuntimeOption::EvalGdbSyncChunks = 128;
bool RuntimeOption::EvalJitStressLease = false;
bool RuntimeOption::EvalJitKeepDbgFiles = false;
bool RuntimeOption::EvalJitEnableRenameFunction = false;
std::set<string, stdltistr> RuntimeOption::DynamicInvokeFunctions;
bool RuntimeOption::EvalJitCmovVarDeref = true;
bool RuntimeOption::EvalJitTransCounters = false;
bool RuntimeOption::EvalJitUseIR = false;
bool RuntimeOption::EvalIRPuntDontInterp = false;
bool RuntimeOption::EvalHHIRMemOpt = false;
uint32 RuntimeOption::EvalHHIRNumFreeRegs = (uint32)-1;
bool RuntimeOption::EvalHHIREnableRematerialization = true;
bool RuntimeOption::EvalHHIREnableCalleeSavedOpt = true;
bool RuntimeOption::EvalHHIREnablePreColoring = true;
bool RuntimeOption::EvalHHIREnableCoalescing = true;
bool RuntimeOption::EvalHHIREnableMmx = true;
bool RuntimeOption::EvalHHIREnableRefCountOpt = true;
bool RuntimeOption::EvalHHIREnableSinking = true;
uint64 RuntimeOption::EvalMaxHHIRTrans = (uint64)-1;
bool RuntimeOption::EvalThreadingJit = false;
bool RuntimeOption::EvalJitDisabledByHphpd = false;
bool RuntimeOption::EvalJitMGeneric = true;
bool RuntimeOption::EvalDumpBytecode = false;
uint32 RuntimeOption::EvalDumpIR = 0;
bool RuntimeOption::EvalDumpTC = false;
bool RuntimeOption::EvalDumpAst = false;
bool RuntimeOption::EvalMapTCHuge = true;
bool RuntimeOption::RecordCodeCoverage = false;
std::string RuntimeOption::CodeCoverageOutputFile;

std::string RuntimeOption::RepoLocalMode;
std::string RuntimeOption::RepoLocalPath;
std::string RuntimeOption::RepoCentralPath;
std::string RuntimeOption::RepoEvalMode;
bool RuntimeOption::RepoCommit = true;
bool RuntimeOption::RepoDebugInfo = true;
// Missing: RuntimeOption::RepoAuthoritative's physical location is
// perf-sensitive.

bool RuntimeOption::SandboxMode = false;
std::string RuntimeOption::SandboxPattern;
std::string RuntimeOption::SandboxHome;
std::string RuntimeOption::SandboxFallback;
std::string RuntimeOption::SandboxConfFile;
std::map<std::string, std::string> RuntimeOption::SandboxServerVariables;
bool RuntimeOption::SandboxFromCommonRoot;
std::string RuntimeOption::SandboxDirectoriesRoot;
std::string RuntimeOption::SandboxLogsRoot;

bool RuntimeOption::EnableDebugger = false;
bool RuntimeOption::EnableDebuggerServer = false;
int RuntimeOption::DebuggerServerPort = 8089;
int RuntimeOption::DebuggerDefaultRpcPort = 8083;
std::string RuntimeOption::DebuggerDefaultRpcAuth;
std::string RuntimeOption::DebuggerRpcHostDomain;
int RuntimeOption::DebuggerDefaultRpcTimeout = 30;
std::string RuntimeOption::DebuggerDefaultSandboxPath;
std::string RuntimeOption::DebuggerStartupDocument;
std::string RuntimeOption::DebuggerUsageLogFile;

std::string RuntimeOption::SendmailPath;
std::string RuntimeOption::MailForceExtraParameters;

int RuntimeOption::PregBacktraceLimit = 100000;
int RuntimeOption::PregRecursionLimit = 100000;
bool RuntimeOption::EnablePregErrorLog = true;

bool RuntimeOption::EnableHotProfiler = true;
int RuntimeOption::ProfilerTraceBuffer = 2000000;
double RuntimeOption::ProfilerTraceExpansion = 1.2;
int RuntimeOption::ProfilerMaxTraceBuffer = 0;

int RuntimeOption::EnableAlternative = 0;

///////////////////////////////////////////////////////////////////////////////

static void setResourceLimit(int resource, Hdf rlimit, const char *nodeName) {
  if (!rlimit[nodeName].getString().empty()) {
    struct rlimit rl;
    getrlimit(resource, &rl);
    rl.rlim_cur = rlimit[nodeName].getInt64();
    if (rl.rlim_max < rl.rlim_cur) {
      rl.rlim_max = rl.rlim_cur;
    }
    int ret = setrlimit(resource, &rl);
    if (ret) {
      Logger::Error("Unable to set %s to %ld: %s (%d)", nodeName, rl.rlim_cur,
                    Util::safe_strerror(errno).c_str(), errno);
    }
  }
}

static void normalizePath(std::string &path) {
  if (!path.empty()) {
    if (path[path.length() - 1] == '/') {
      path = path.substr(0, path.length() - 1);
    }
    if (path[0] != '/') {
      path = std::string("/") + path;
    }
  }
}

static bool matchHdfPattern(const std::string &value, Hdf hdfPattern) {
  string pattern = hdfPattern.getString();
  if (!pattern.empty()) {
    Variant ret = preg_match(String(pattern.c_str(), pattern.size(),
                                    AttachLiteral),
                             String(value.c_str(), value.size(),
                                    AttachLiteral));
    if (ret.toInt64() <= 0) {
      return false;
    }
  }
  return true;
}

static inline bool evalJitDefault() {
  // Only use JIT for HHVM
  if (!hhvm) {
    return false;
  }

  // --mode server or --mode daemon
  // run long enough to justify JIT
  if (RuntimeOption::serverExecutionMode()) {
    return true;
  }

  // JIT explicitly turned on via .hhvm-jit file
  static const char* path = "/.hhvm-jit";
  struct stat dummy;
  return stat(path, &dummy) == 0;
}

void RuntimeOption::Load(Hdf &config, StringVec *overwrites /* = NULL */,
                         bool empty /* = false */) {
  // Machine metrics
  string hostname, tier, cpu;
  {
    Hdf machine = config["Machine"];

    hostname = machine["name"].getString();
    if (hostname.empty()) {
      hostname = Process::GetHostName();
    }

    tier = machine["tier"].getString();

    cpu = machine["cpu"].getString();
    if (cpu.empty()) {
      cpu = Process::GetCPUModel();
    }
  }

  // Tier overwrites
  {
    Hdf tiers = config["Tiers"];
    for (Hdf hdf = tiers.firstChild(); hdf.exists(); hdf = hdf.next()) {
      if (matchHdfPattern(hostname, hdf["machine"]) &&
          matchHdfPattern(tier, hdf["tier"]) &&
          matchHdfPattern(cpu, hdf["cpu"])) {
        Tier = hdf.getName();
        config.copy(hdf["overwrite"]);
        // no break here, so we can continue to match more overwrites
      }
      hdf["overwrite"].setVisited(); // avoid lint complaining
    }
  }

  // More overwrites
  if (overwrites) {
    for (unsigned int i = 0; i < overwrites->size(); i++) {
      config.fromString(overwrites->at(i).c_str());
    }
  }

  PidFile = config["PidFile"].getString("www.pid");

  config["DynamicInvokeFunctions"].get(DynamicInvokeFunctions);

  {
    Hdf logger = config["Log"];
    if (logger["Level"] == "None") {
      Logger::LogLevel = Logger::LogNone;
    } else if (logger["Level"] == "Error") {
      Logger::LogLevel = Logger::LogError;
    } else if (logger["Level"] == "Warning") {
      Logger::LogLevel = Logger::LogWarning;
    } else if (logger["Level"] == "Info") {
      Logger::LogLevel = Logger::LogInfo;
    } else if (logger["Level"] == "Verbose") {
      Logger::LogLevel = Logger::LogVerbose;
    }
    Logger::LogHeader = logger["Header"].getBool();
    bool logInjectedStackTrace = logger["InjectedStackTrace"].getBool();
    if (logInjectedStackTrace) {
      Logger::SetTheLogger(new ExtendedLogger());
      ExtendedLogger::EnabledByDefault = true;
    }
    Logger::LogNativeStackTrace = logger["NativeStackTrace"].getBool(true);
    Logger::MaxMessagesPerRequest =
      logger["MaxMessagesPerRequest"].getInt32(-1);

    Logger::UseSyslog = logger["UseSyslog"].getBool(false);
    Logger::UseLogFile = logger["UseLogFile"].getBool(true);
    Logger::UseCronolog = logger["UseCronolog"].getBool(false);
    if (Logger::UseLogFile) {
      LogFile = logger["File"].getString();
      if (LogFile[0] == '|') Logger::IsPipeOutput = true;
      LogFileSymLink = logger["SymLink"].getString();
    }
    Logger::DropCacheChunkSize =
      logger["DropCacheChunkSize"].getInt32(1 << 20);
    AlwaysEscapeLog = logger["AlwaysEscapeLog"].getBool(false);

    Hdf aggregator = logger["Aggregator"];
    Logger::UseLogAggregator = aggregator.getBool();
    LogAggregatorFile = aggregator["File"].getString();
    LogAggregatorDatabase = aggregator["Database"].getString();
    LogAggregatorSleepSeconds = aggregator["SleepSeconds"].getInt16(10);

    AlwaysLogUnhandledExceptions =
      logger["AlwaysLogUnhandledExceptions"].getBool(true);
    NoSilencer = logger["NoSilencer"].getBool();
    EnableApplicationLog = logger["ApplicationLog"].getBool(true);
    RuntimeErrorReportingLevel =
      logger["RuntimeErrorReportingLevel"].getInt32(ErrorConstants::HPHP_ALL);

    AccessLogDefaultFormat = logger["AccessLogDefaultFormat"].
      getString("%h %l %u %t \"%r\" %>s %b");
    {
      Hdf access = logger["Access"];
      for (Hdf hdf = access.firstChild(); hdf.exists();
           hdf = hdf.next()) {
        string fname = hdf["File"].getString();
        if (fname.empty()) {
          continue;
        }
        string symLink = hdf["SymLink"].getString();
        AccessLogs.
          push_back(AccessLogFileData(fname, symLink, hdf["Format"].
                                      getString(AccessLogDefaultFormat)));
      }
    }

    AdminLogFormat = logger["AdminLog.Format"].getString("%h %t %s %U");
    AdminLogFile = logger["AdminLog.File"].getString();
    AdminLogSymLink = logger["AdminLog.SymLink"].getString();
  }
  {
    Hdf error = config["ErrorHandling"];

    /* Remove this, once its removed from production configs */
    (void)error["NoInfiniteLoopDetection"].getBool();

    MaxSerializedStringSize =
      error["MaxSerializedStringSize"].getInt32(64 * 1024 * 1024);
    CallUserHandlerOnFatals = error["CallUserHandlerOnFatals"].getBool(true);
    ThrowExceptionOnBadMethodCall =
      error["ThrowExceptionOnBadMethodCall"].getBool(true);
    MaxLoopCount = error["MaxLoopCount"].getInt32(0);
    NoInfiniteRecursionDetection =
      error["NoInfiniteRecursionDetection"].getBool();
    ThrowBadTypeExceptions = error["ThrowBadTypeExceptions"].getBool();
    ThrowTooManyArguments = error["ThrowTooManyArguments"].getBool();
    WarnTooManyArguments = error["WarnTooManyArguments"].getBool();
    ThrowMissingArguments = error["ThrowMissingArguments"].getBool();
    ThrowInvalidArguments = error["ThrowInvalidArguments"].getBool();
    EnableHipHopErrors = error["EnableHipHopErrors"].getBool(true);
    AssertActive = error["AssertActive"].getBool();
    AssertWarning = error["AssertWarning"].getBool();
    NoticeFrequency = error["NoticeFrequency"].getInt32(1);
    WarningFrequency = error["WarningFrequency"].getInt32(1);
  }
  {
    Hdf rlimit = config["ResourceLimit"];
    setResourceLimit(RLIMIT_CORE,   rlimit, "CoreFileSize");
    setResourceLimit(RLIMIT_NOFILE, rlimit, "MaxSocket");
    setResourceLimit(RLIMIT_DATA,   rlimit, "RSS");
    MaxRSS = rlimit["MaxRSS"].getInt64(0);
    SocketDefaultTimeout = rlimit["SocketDefaultTimeout"].getInt16(5);
    MaxRSSPollingCycle = rlimit["MaxRSSPollingCycle"].getInt64(0);
    DropCacheCycle = rlimit["DropCacheCycle"].getInt64(0);
    MaxSQLRowCount = rlimit["MaxSQLRowCount"].getInt64(0);
    MaxMemcacheKeyCount = rlimit["MaxMemcacheKeyCount"].getInt64(0);
    SerializationSizeLimit =
      rlimit["SerializationSizeLimit"].getInt64(StringData::MaxSize);
    StringOffsetLimit = rlimit["StringOffsetLimit"].getInt64(10 * 1024 * 1024);
  }
  {
    Hdf server = config["Server"];
    Host = server["Host"].getString();
    DefaultServerNameSuffix = server["DefaultServerNameSuffix"].getString();
    ServerIP = server["IP"].getString();
    ServerPrimaryIP = Util::GetPrimaryIP();
    ServerPort = server["Port"].getInt16(80);
    ServerBacklog = server["Backlog"].getInt16(128);
    ServerConnectionLimit = server["ConnectionLimit"].getInt16(0);
    ServerThreadCount = server["ThreadCount"].getInt32(50);
    ServerThreadRoundRobin = server["ThreadRoundRobin"].getBool();
    ServerThreadDropCacheTimeoutSeconds =
      server["ThreadDropCacheTimeoutSeconds"].getInt32(0);
    ServerThreadJobLIFO = server["ThreadJobLIFO"].getBool();
    ServerThreadDropStack = server["ThreadDropStack"].getBool();
    ServerHttpSafeMode = server["HttpSafeMode"].getBool();
    ServerStatCache = server["StatCache"].getBool(true);
    RequestTimeoutSeconds = server["RequestTimeoutSeconds"].getInt32(0);
    ServerMemoryHeadRoom = server["MemoryHeadRoom"].getInt64(0);
    RequestMemoryMaxBytes = server["RequestMemoryMaxBytes"].getInt64(INT64_MAX);
    ResponseQueueCount = server["ResponseQueueCount"].getInt32(0);
    if (ResponseQueueCount <= 0) {
      ResponseQueueCount = ServerThreadCount / 10;
      if (ResponseQueueCount <= 0) ResponseQueueCount = 1;
    }
    ServerGracefulShutdownWait = server["GracefulShutdownWait"].getInt16(0);
    ServerHarshShutdown = server["HarshShutdown"].getBool(true);
    ServerEvilShutdown = server["EvilShutdown"].getBool(true);
    ServerDanglingWait = server["DanglingWait"].getInt16(0);
    ServerShutdownListenWait = server["ShutdownListenWait"].getInt16(0);
    ServerShutdownListenNoWork = server["ShutdownListenNoWork"].getInt16(-1);
    if (ServerGracefulShutdownWait < ServerDanglingWait) {
      ServerGracefulShutdownWait = ServerDanglingWait;
    }
    GzipCompressionLevel = server["GzipCompressionLevel"].getInt16(3);

    ForceCompressionURL    = server["ForceCompression"]["URL"].getString();
    ForceCompressionCookie = server["ForceCompression"]["Cookie"].getString();
    ForceCompressionParam  = server["ForceCompression"]["Param"].getString();

    EnableMagicQuotesGpc = server["EnableMagicQuotesGpc"].getBool();
    EnableKeepAlive = server["EnableKeepAlive"].getBool(true);
    ExposeHPHP = server["ExposeHPHP"].getBool(true);
    ExposeXFBServer = server["ExposeXFBServer"].getBool(false);
    ExposeXFBDebug = server["ExposeXFBDebug"].getBool(false);
    XFBDebugSSLKey = server["XFBDebugSSLKey"].getString("");
    ConnectionTimeoutSeconds = server["ConnectionTimeoutSeconds"].getInt16(-1);
    EnableOutputBuffering = server["EnableOutputBuffering"].getBool();
    OutputHandler = server["OutputHandler"].getString();
    ImplicitFlush = server["ImplicitFlush"].getBool();
    EnableEarlyFlush = server["EnableEarlyFlush"].getBool(true);
    ForceChunkedEncoding = server["ForceChunkedEncoding"].getBool();
    MaxPostSize = (server["MaxPostSize"].getInt32(100)) * (1LL << 20);
    AlwaysPopulateRawPostData =
      server["AlwaysPopulateRawPostData"].getBool(true);
    LibEventSyncSend = server["LibEventSyncSend"].getBool(true);
    TakeoverFilename = server["TakeoverFilename"].getString();
    ExpiresActive = server["ExpiresActive"].getBool(true);
    ExpiresDefault = server["ExpiresDefault"].getInt32(2592000);
    if (ExpiresDefault < 0) ExpiresDefault = 2592000;
    DefaultCharsetName = server["DefaultCharsetName"].getString("utf-8");

    RequestBodyReadLimit = server["RequestBodyReadLimit"].getInt32(-1);

    EnableSSL = server["EnableSSL"].getBool();
    SSLPort = server["SSLPort"].getInt16(443);
    SSLCertificateFile = server["SSLCertificateFile"].getString();
    SSLCertificateKeyFile = server["SSLCertificateKeyFile"].getString();
    SSLCertificateDir = server["SSLCertificateDir"].getString();

    string srcRoot = Util::normalizeDir(server["SourceRoot"].getString());
    if (!srcRoot.empty()) SourceRoot = srcRoot;
    FileCache::SourceRoot = SourceRoot;

    server["IncludeSearchPaths"].get(IncludeSearchPaths);
    for (unsigned int i = 0; i < IncludeSearchPaths.size(); i++) {
      IncludeSearchPaths[i] = Util::normalizeDir(IncludeSearchPaths[i]);
    }
    IncludeSearchPaths.insert(IncludeSearchPaths.begin(), "./");

    FileCache = server["FileCache"].getString();
    DefaultDocument = server["DefaultDocument"].getString();
    ErrorDocument404 = server["ErrorDocument404"].getString();
    normalizePath(ErrorDocument404);
    ForbiddenAs404 = server["ForbiddenAs404"].getBool();
    ErrorDocument500 = server["ErrorDocument500"].getString();
    normalizePath(ErrorDocument500);
    FatalErrorMessage = server["FatalErrorMessage"].getString();
    FontPath = Util::normalizeDir(server["FontPath"].getString());
    EnableStaticContentCache =
      server["EnableStaticContentCache"].getBool(true);
    EnableStaticContentFromDisk =
      server["EnableStaticContentFromDisk"].getBool(true);
    EnableOnDemandUncompress =
      server["EnableOnDemandUncompress"].getBool(true);
    EnableStaticContentMMap =
      server["EnableStaticContentMMap"].getBool(true);
    if (EnableStaticContentMMap) {
      EnableOnDemandUncompress = true;
    }
    RTTIDirectory =
      Util::normalizeDir(server["RTTIDirectory"].getString("/tmp/"));
    EnableCliRTTI = server["EnableCliRTTI"].getBool();
    Utf8izeReplace = server["Utf8izeReplace"].getBool(true);

    StartupDocument = server["StartupDocument"].getString();
    normalizePath(StartupDocument);
    WarmupDocument = server["WarmupDocument"].getString();
    RequestInitFunction = server["RequestInitFunction"].getString();
    RequestInitDocument = server["RequestInitDocument"].getString();
    server["ThreadDocuments"].get(ThreadDocuments);
    for (unsigned int i = 0; i < ThreadDocuments.size(); i++) {
      normalizePath(ThreadDocuments[i]);
    }
    server["ThreadLoopDocuments"].get(ThreadLoopDocuments);
    for (unsigned int i = 0; i < ThreadLoopDocuments.size(); i++) {
      normalizePath(ThreadLoopDocuments[i]);
    }

    SafeFileAccess = server["SafeFileAccess"].getBool();
    server["AllowedDirectories"].get(AllowedDirectories);
    for (unsigned int i = 0; i < AllowedDirectories.size(); i++) {
      string &directory = AllowedDirectories[i];
      char resolved_path[PATH_MAX];
      if (realpath(directory.c_str(), resolved_path) &&
          directory != resolved_path) {
        RuntimeOption::AllowedDirectories.push_back(resolved_path);
      }
    }

    WhitelistExec = server["WhitelistExec"].getBool();
    WhitelistExecWarningOnly = server["WhitelistExecWarningOnly"].getBool();
    server["AllowedExecCmds"].get(AllowedExecCmds);

    server["AllowedFiles"].get(AllowedFiles);

    server["ForbiddenFileExtensions"].get(ForbiddenFileExtensions);

    LockCodeMemory = server["LockCodeMemory"].getBool(false);
    EnableMemoryManager = server["EnableMemoryManager"].getBool(true);
    if (!EnableMemoryManager) {
      MemoryManager::TheMemoryManager()->disable();
    }
    CheckMemory = server["CheckMemory"].getBool();
    MaxArrayChain = server["MaxArrayChain"].getInt32(INT_MAX);
    UseHphpArray = server["UseHphpArray"].getBool(hhvm);

    // TODO: Task #1154042: This runtime option is no longer used, remove it
    UseSmallArray = server["UseSmallArray"].getBool(false);

    UseVectorArray = server["UseVectorArray"].getBool(true);
    StrictCollections = server["StrictCollections"].getBool(false);
    WarnOnCollectionToArray = server["WarnOnCollectionToArray"].getBool(false);
    UseDirectCopy = server["UseDirectCopy"].getBool(false);
    AlwaysUseRelativePath = server["AlwaysUseRelativePath"].getBool(false);

    Hdf apc = server["APC"];
    EnableApc = apc["EnableApc"].getBool(true);
    EnableConstLoad = apc["EnableConstLoad"].getBool(false);
    ForceConstLoadToAPC = apc["ForceConstLoadToAPC"].getBool(true);
    ApcPrimeLibrary = apc["PrimeLibrary"].getString();
    ApcLoadThread = apc["LoadThread"].getInt16(2);
    apc["CompletionKeys"].get(ApcCompletionKeys);

    string apcTableType = apc["TableType"].getString("concurrent");
    if (strcasecmp(apcTableType.c_str(), "concurrent") == 0) {
      ApcTableType = ApcConcurrentTable;
    } else {
      throw InvalidArgumentException("apc table type",
                                     "Invalid table type");
    }
    EnableApcSerialize = apc["EnableApcSerialize"].getBool(true);
    ApcExpireOnSets = apc["ExpireOnSets"].getBool();
    ApcPurgeFrequency = apc["PurgeFrequency"].getInt32(4096);
    ApcPurgeRate = apc["PurgeRate"].getInt32(-1);

    ApcAllowObj = apc["AllowObject"].getBool();
    ApcTTLLimit = apc["TTLLimit"].getInt32(-1);
    Hdf fileStorage = apc["FileStorage"];
    ApcUseFileStorage = fileStorage["Enable"].getBool();
    ApcFileStorageChunkSize = fileStorage["ChunkSize"].getInt64(1LL << 29);
    ApcFileStorageMaxSize = fileStorage["MaxSize"].getInt64(1LL << 32);
    ApcFileStoragePrefix = fileStorage["Prefix"].getString("/tmp/apc_store");
    ApcFileStorageFlagKey = fileStorage["FlagKey"].getString("_madvise_out");
    ApcFileStorageAdviseOutPeriod =
      fileStorage["AdviseOutPeriod"].getInt32(1800);
    ApcFileStorageKeepFileLinked = fileStorage["KeepFileLinked"].getBool();

    ApcConcurrentTableLockFree = apc["ConcurrentTableLockFree"].getBool(false);
    ApcKeyMaturityThreshold = apc["KeyMaturityThreshold"].getInt32(20);
    ApcMaximumCapacity = apc["MaximumCapacity"].getInt64(0);
    ApcKeyFrequencyUpdatePeriod = apc["KeyFrequencyUpdatePeriod"].
      getInt32(1000);

    apc["NoTTLPrefix"].get(ApcNoTTLPrefix);

    Hdf dns = server["DnsCache"];
    EnableDnsCache = dns["Enable"].getBool();
    DnsCacheTTL = dns["TTL"].getInt32(600); // 10 minutes
    DnsCacheKeyMaturityThreshold = dns["KeyMaturityThreshold"].getInt32(20);
    DnsCacheMaximumCapacity = dns["MaximumCapacity"].getInt64(0);
    DnsCacheKeyFrequencyUpdatePeriod = dns["KeyFrequencyUpdatePeriod"].
      getInt32(1000);

    Hdf upload = server["Upload"];
    UploadMaxFileSize =
      (upload["UploadMaxFileSize"].getInt32(100)) * (1LL << 20);
    UploadTmpDir = upload["UploadTmpDir"].getString("/tmp");
    RuntimeOption::AllowedDirectories.push_back(UploadTmpDir);
    EnableFileUploads = upload["EnableFileUploads"].getBool(true);
    EnableUploadProgress = upload["EnableUploadProgress"].getBool();
    Rfc1867Freq = upload["Rfc1867Freq"].getInt32(256 * 1024);
    if (Rfc1867Freq < 0) Rfc1867Freq = 256 * 1024;
    Rfc1867Prefix = upload["Rfc1867Prefix"].getString("vupload_");
    Rfc1867Name = upload["Rfc1867Name"].getString("video_ptoken");

    ImageMemoryMaxBytes = server["ImageMemoryMaxBytes"].getInt64(0);
    if (ImageMemoryMaxBytes == 0) {
      ImageMemoryMaxBytes = UploadMaxFileSize * 2;
    }
    SharedStores::Create();

    LightProcessFilePrefix =
      server["LightProcessFilePrefix"].getString("./lightprocess");
    LightProcessCount = server["LightProcessCount"].getInt32(0);

    InjectedStackTrace = server["InjectedStackTrace"].getBool(true);
    InjectedStackTraceLimit = server["InjectedStackTraceLimit"].getInt32(-1);

    ForceServerNameToHeader = server["ForceServerNameToHeader"].getBool();

    EnableCufAsync = server["EnableCufAsync"].getBool(false);

    ServerUser = server["User"].getString("");
  }
  {
    Hdf hosts = config["VirtualHost"];
    if (hosts.exists()) {
      for (Hdf hdf = hosts.firstChild(); hdf.exists(); hdf = hdf.next()) {
        if (hdf.getName() == "default") {
          VirtualHost::GetDefault().init(hdf);
        } else {
          VirtualHostPtr host(new VirtualHost(hdf));
          VirtualHosts.push_back(host);
        }
      }
      for (unsigned int i = 0; i < VirtualHosts.size(); i++) {
        if (!VirtualHosts[i]->valid()) {
          throw InvalidArgumentException("virtual host",
                                         "missing prefix or pattern");
        }
      }
    }
  }
  {
    Hdf ipblocks = config["IpBlockMap"];
    IpBlocks = IpBlockMapPtr(new IpBlockMap(ipblocks));
  }
  {
    Hdf satellites = config["Satellites"];
    if (satellites.exists()) {
      for (Hdf hdf = satellites.firstChild(); hdf.exists(); hdf = hdf.next()) {
        SatelliteServerInfoPtr satellite(new SatelliteServerInfo(hdf));
        SatelliteServerInfos.push_back(satellite);
        if (satellite->getType() == SatelliteServer::KindOfRPCServer) {
          XboxPassword = satellite->getPassword();
          XboxPasswords = satellite->getPasswords();
        }
      }
    }
  }
  {
    Hdf xbox = config["Xbox"];
    XboxServerThreadCount = xbox["ServerInfo.ThreadCount"].getInt32(10);
    XboxServerMaxQueueLength =
      xbox["ServerInfo.MaxQueueLength"].getInt32(INT_MAX);
    if (XboxServerMaxQueueLength < 0) XboxServerMaxQueueLength = INT_MAX;
    XboxServerPort = xbox["ServerInfo.Port"].getInt32(0);
    XboxDefaultLocalTimeoutMilliSeconds =
      xbox["DefaultLocalTimeoutMilliSeconds"].getInt32(500);
    XboxDefaultRemoteTimeoutSeconds =
      xbox["DefaultRemoteTimeoutSeconds"].getInt32(5);
    XboxServerInfoMaxRequest = xbox["ServerInfo.MaxRequest"].getInt32(500);
    XboxServerInfoDuration = xbox["ServerInfo.MaxDuration"].getInt32(120);
    XboxServerInfoWarmupDoc = xbox["ServerInfo.WarmupDocument"].get("");
    XboxServerInfoReqInitFunc = xbox["ServerInfo.RequestInitFunction"].get("");
    XboxServerInfoReqInitDoc = xbox["ServerInfo.RequestInitDocument"].get("");
    XboxServerInfoAlwaysReset = xbox["ServerInfo.AlwaysReset"].getBool(false);
    XboxServerLogInfo = xbox["ServerInfo.LogInfo"].getBool(false);
    XboxProcessMessageFunc =
      xbox["ProcessMessageFunc"].get("xbox_process_message");
  }
  {
    Hdf pagelet = config["PageletServer"];
    PageletServerThreadCount = pagelet["ThreadCount"].getInt32(0);
    PageletServerThreadRoundRobin = pagelet["ThreadRoundRobin"].getBool();
    PageletServerThreadDropStack = pagelet["ThreadDropStack"].getBool();
    PageletServerThreadDropCacheTimeoutSeconds =
      pagelet["ThreadDropCacheTimeoutSeconds"].getInt32(0);
    PageletServerQueueLimit = pagelet["QueueLimit"].getInt32(0);
  }
  {
    FiberCount = config["Fiber.ThreadCount"].getInt32(Process::GetCPUCount());
  }
  {
    Hdf content = config["StaticFile"];
    content["Extensions"].get(StaticFileExtensions);
    content["Generators"].get(StaticFileGenerators);

    Hdf matches = content["FilesMatch"];
    if (matches.exists()) {
      for (Hdf hdf = matches.firstChild(); hdf.exists(); hdf = hdf.next()) {
        FilesMatches.push_back(FilesMatchPtr(new FilesMatch(hdf)));
      }
    }
  }
  {
    Hdf admin = config["AdminServer"];
    AdminServerPort = admin["Port"].getInt16(8088);
    AdminThreadCount = admin["ThreadCount"].getInt32(1);
    AdminPassword = admin["Password"].getString();
    admin["Passwords"].get(AdminPasswords);
  }
  {
    Hdf proxy = config["Proxy"];
    ProxyOrigin = proxy["Origin"].getString();
    ProxyRetry = proxy["Retry"].getInt16(3);
    UseServeURLs = proxy["ServeURLs"].getBool();
    proxy["ServeURLs"].get(ServeURLs);
    UseProxyURLs = proxy["ProxyURLs"].getBool();
    ProxyPercentage = proxy["Percentage"].getByte(0);
    proxy["ProxyURLs"].get(ProxyURLs);
    proxy["ProxyPatterns"].get(ProxyPatterns);
  }
  {
    Hdf mysql = config["MySQL"];
    MySQLReadOnly = mysql["ReadOnly"].getBool();
    MySQLLocalize = mysql["Localize"].getBool();
    MySQLConnectTimeout = mysql["ConnectTimeout"].getInt32(1000);
    MySQLReadTimeout = mysql["ReadTimeout"].getInt32(1000);
    MySQLWaitTimeout = mysql["WaitTimeout"].getInt32(-1);
    MySQLSlowQueryThreshold = mysql["SlowQueryThreshold"].getInt32(1000);
    MySQLKillOnTimeout = mysql["KillOnTimeout"].getBool();
    MySQLMaxRetryOpenOnFail = mysql["MaxRetryOpenOnFail"].getInt32(1);
    MySQLMaxRetryQueryOnFail = mysql["MaxRetryQueryOnFail"].getInt32(1);
  }
  {
    Hdf http = config["Http"];
    HttpDefaultTimeout = http["DefaultTimeout"].getInt32(30);
    HttpSlowQueryThreshold = http["SlowQueryThreshold"].getInt32(5000);
  }
  {
    Hdf debug = config["Debug"];
    NativeStackTrace = debug["NativeStackTrace"].getBool();
    StackTrace::Enabled = NativeStackTrace;
    TranslateLeakStackTrace = debug["TranslateLeakStackTrace"].getBool();
    FullBacktrace = debug["FullBacktrace"].getBool();
    ServerStackTrace = debug["ServerStackTrace"].getBool();
    ServerErrorMessage = debug["ServerErrorMessage"].getBool();
    TranslateSource = debug["TranslateSource"].getBool();
    RecordInput = debug["RecordInput"].getBool();
    ClearInputOnSuccess = debug["ClearInputOnSuccess"].getBool(true);
    ProfilerOutputDir = debug["ProfilerOutputDir"].getString("/tmp");
    CoreDumpEmail = debug["CoreDumpEmail"].getString();
    if (!CoreDumpEmail.empty()) {
      StackTrace::ReportEmail = CoreDumpEmail;
    }
    CoreDumpReport = debug["CoreDumpReport"].getBool(true);
    if (CoreDumpReport) {
      StackTrace::InstallReportOnErrors();
    }
    std::string reportDirectory = debug["CoreDumpReportDirectory"].getString();
    if (!reportDirectory.empty()) {
      StackTraceBase::ReportDirectory = reportDirectory;
    }
    LocalMemcache = debug["LocalMemcache"].getBool();
    MemcacheReadOnly = debug["MemcacheReadOnly"].getBool();

    {
      Hdf simpleCounter = debug["SimpleCounter"];
      SimpleCounter::SampleStackCount =
        simpleCounter["SampleStackCount"].getInt32(0);
      SimpleCounter::SampleStackDepth =
        simpleCounter["SampleStackDepth"].getInt32(5);
    }
  }
  {
    Hdf stats = config["Stats"];
    EnableStats = stats.getBool(); // main switch

    EnableWebStats = stats["Web"].getBool();
    EnableMemoryStats = stats["Memory"].getBool();
    EnableMallocStats = stats["Malloc"].getBool();
    EnableAPCStats = stats["APC"].getBool();
    EnableAPCKeyStats = stats["APCKey"].getBool();
    EnableMemcacheStats = stats["Memcache"].getBool();
    EnableMemcacheKeyStats = stats["MemcacheKey"].getBool();
    EnableSQLStats = stats["SQL"].getBool();
    EnableSQLTableStats = stats["SQLTable"].getBool();
    EnableNetworkIOStatus = stats["NetworkIO"].getBool();

    if (EnableStats && EnableMallocStats) {
      LeakDetectable::EnableMallocStats(true);
    }

    StatsXSL = stats["XSL"].getString();
    StatsXSLProxy = stats["XSLProxy"].getString();

    StatsSlotDuration = stats["SlotDuration"].getInt32(10 * 60); // 10 minutes
    StatsMaxSlot = stats["MaxSlot"].getInt32(12 * 6); // 12 hours

    {
      Hdf apcSize = stats["APCSize"];
      EnableAPCSizeStats = apcSize["Enable"].getBool();
      EnableAPCSizeGroup = apcSize["Group"].getBool();
      apcSize["SpecialPrefix"].get(APCSizeSpecialPrefix);
      for (unsigned int i = 0; i < APCSizeSpecialPrefix.size(); i++) {
        string &prefix = APCSizeSpecialPrefix[i];
        string prefixReplace = prefix + "{A}";
        APCSizePrefixReplace.push_back(prefixReplace);
      }
      apcSize["SpecialMiddle"].get(APCSizeSpecialMiddle);
      for (unsigned int i = 0; i < APCSizeSpecialMiddle.size(); i++) {
        string &middle = APCSizeSpecialMiddle[i];
        string middleReplace = "{A}" + middle + "{A}";
        APCSizeMiddleReplace.push_back(middleReplace);
      }
      apcSize["SkipPrefix"].get(APCSizeSkipPrefix);
      EnableAPCSizeDetail = apcSize["Individual"].getBool();
      EnableAPCFetchStats = apcSize["FetchStats"].getBool();
      if (EnableAPCFetchStats) EnableAPCSizeDetail = true;
      if (EnableAPCSizeDetail) EnableAPCSizeGroup = true;
      APCSizeCountPrime = apcSize["CountPrime"].getBool();
    }

    EnableHotProfiler = stats["EnableHotProfiler"].getBool(true);
    ProfilerTraceBuffer = stats["ProfilerTraceBuffer"].getInt32(2000000);
    ProfilerTraceExpansion = stats["ProfilerTraceExpansion"].getDouble(1.2);
    ProfilerMaxTraceBuffer = stats["ProfilerMaxTraceBuffer"].getInt32(0);
  }
  {
    config["ServerVariables"].get(ServerVariables);
    config["EnvVariables"].get(EnvVariables);
  }
  {
    Hdf eval = config["Eval"];
    EnableHipHopSyntax = eval["EnableHipHopSyntax"].getBool();
    EnableHipHopExperimentalSyntax =
      eval["EnableHipHopExperimentalSyntax"].getBool();
    EnableShortTags= eval["EnableShortTags"].getBool(true);
    if (EnableShortTags) ScannerType |= Scanner::AllowShortTags;
    else ScannerType &= ~Scanner::AllowShortTags;

    EnableAspTags = eval["EnableAspTags"].getBool();
    if (EnableAspTags) ScannerType |= Scanner::AllowAspTags;
    else ScannerType &= ~Scanner::AllowAspTags;

    EnableXHP = eval["EnableXHP"].getBool(true);
    EnableObjDestructCall = eval["EnableObjDestructCall"].getBool(false);
    EnableEvalOptimization = eval["EnableEvalOptimization"].getBool(true);
    EvalScalarValueExprLimit = eval["EvalScalarValueExprLimit"].getInt32(64);
    MaxUserFunctionId = eval["MaxUserFunctionId"].getInt32(2 * 65536);
    CheckSymLink = eval["CheckSymLink"].getBool(false);
    NativeXHP = eval["NativeXHP"].getBool(true);
    if (EnableXHP && !NativeXHP) ScannerType |= Scanner::PreprocessXHP;
    else ScannerType &= ~Scanner::PreprocessXHP;

    EnableAlternative = eval["EnableAlternative"].getInt32(0);

    EnableFinallyStatement = eval["EnableFinallyStatement"].getBool();

#ifdef TAINTED
    EnableTaintWarnings = eval["EnableTaintWarnings"].getBool();
    TaintTraceMaxStrlen = eval["TaintTraceMaxStrlen"].getInt32(127);
#endif

    EnableStrict = eval["EnableStrict"].getBool();
    StrictLevel = eval["StrictLevel"].getInt32(1); // StrictBasic
    StrictFatal = eval["StrictFatal"].getBool();
    EvalVMStackElms = eval["VMStackElms"].getUInt64(kEvalVMStackElmsDefault);
    EvalVMInitialGlobalTableSize =
      eval["VMInitialGlobalTableSize"].getUInt64(
        kEvalVMInitialGlobalTableSizeDefault);
    EvalJit = eval["Jit"].getBool(evalJitDefault());
    EvalAllowHhas = eval["AllowHhas"].getBool(false);
    EvalJitNoGdb = eval["JitNoGdb"].getBool(true);
    EvalProfileBC = eval["ProfileBC"].getBool(false);
    EvalProfileHWEnable = eval["ProfileHWEnable"].getBool(true);
    EvalProfileHWEvents = eval["ProfileHWEvents"].getString();
    EvalJitTrampolines =
      eval["JitTrampolines"].getBool(JIT_TRAMPOLINES_DEFAULT);
    EvalGdbSyncChunks = eval["GdbSyncChunks"].getInt32(128);
    EvalThreadingJit = eval["ThreadingJit"].getBool(false);
    EvalJitStressLease = eval["JitStressLease"].getBool(false);
    EvalJitKeepDbgFiles = eval["JitKeepDbgFiles"].getBool(false);
    EvalJitEnableRenameFunction =
      eval["JitEnableRenameFunction"].getBool(false) || !EvalJit;
    EvalJitDisabledByHphpd = eval["EvalJitDisabledByHphpd"].getBool(false);
    EvalJitCmovVarDeref = eval["JitCmovVarDeref"].getBool(true);
    EvalJitTransCounters = eval["JitTransCounters"].getBool(false);
    EvalJitProfilePath = eval["JitProfilePath"].getString();
    EvalJitStressTypePredPercent = eval["JitStressTypePredPercent"].getInt32(0);
    EvalJitProfileRecord = eval["JitProfileRecord"].getBool(false);
    EvalJitWarmupRequests = eval["JitWarmupRequests"].getInt32(kDefaultWarmupRequests);
    EvalJitMGeneric = eval["JitMGeneric"].getBool(true);
    EvalJitUseIR = eval["JitUseIR"].getBool(false);
    EvalIRPuntDontInterp = eval["IRPuntDontInterp"].getBool(false);
    EvalHHIRMemOpt = eval["HHIRMemOpt"].getBool(true);
    EvalHHIRNumFreeRegs = eval["HHIRNumFreeRegs"].getUInt32(-1);
    EvalHHIREnableRematerialization = eval["HHIREnableRematerialization"].getBool(true);
    EvalHHIREnableCalleeSavedOpt = eval["HHIREnableCalleeSavedOpt"].getBool(true);
    EvalHHIREnablePreColoring = eval["HHIREnablePreColoring"].getBool(true);
    EvalHHIREnableCoalescing = eval["HHIREnableCoalescing"].getBool(true);
    EvalHHIREnableMmx = eval["HHIREnableMmx"].getBool(true);
    EvalHHIREnableRefCountOpt = eval["HHIREnableRefCountOpt"].getBool(true);
    EvalHHIREnableSinking = eval["HHIREnableSinking"].getBool(true);
    EvalMaxHHIRTrans = eval["MaxHHIRTrans"].getUInt64(-1);
    EnableEmitSwitch = eval["EnableEmitSwitch"].getBool(!EvalJitUseIR);
    EvalDumpBytecode = eval["DumpBytecode"].getBool(false);
    EvalDumpIR = eval["DumpIR"].getUInt32(0);
    EvalDumpTC = eval["DumpTC"].getBool(false);
    EvalDumpAst = eval["DumpAst"].getBool(false);
    EvalMapTCHuge = eval["MapTCHuge"].getBool(true);
    RecordCodeCoverage = eval["RecordCodeCoverage"].getBool();
    if (EvalJit && RecordCodeCoverage) {
      throw InvalidArgumentException(
        "code coverage", "Code coverage is not supported for Eval.Jit=true");
    }
    if (RecordCodeCoverage) CheckSymLink = true;
    CodeCoverageOutputFile = eval["CodeCoverageOutputFile"].getString();
    {
      Hdf debugger = eval["Debugger"];
      EnableDebugger = debugger["EnableDebugger"].getBool();
      EnableDebuggerServer = debugger["EnableDebuggerServer"].getBool();
      DebuggerServerPort = debugger["Port"].getInt16(8089);
      DebuggerDefaultSandboxPath = debugger["DefaultSandboxPath"].getString();
      DebuggerStartupDocument = debugger["StartupDocument"].getString();
      DebuggerUsageLogFile = debugger["UsageLogFile"].getString();

      DebuggerDefaultRpcPort = debugger["RPC.DefaultPort"].getInt16(8083);
      DebuggerDefaultRpcAuth = debugger["RPC.DefaultAuth"].getString();
      DebuggerRpcHostDomain = debugger["RPC.HostDomain"].getString();
      DebuggerDefaultRpcTimeout = debugger["RPC.DefaultTimeout"].getInt32(30);
    }
  }
  {
    Hdf repo = config["Repo"];
    {
      Hdf repoLocal = repo["Local"];
      // Repo.Local.Mode.
      RepoLocalMode = repoLocal["Mode"].getString();
      if (!empty && RepoLocalMode.empty()) {
        const char* HHVM_REPO_LOCAL_MODE = getenv("HHVM_REPO_LOCAL_MODE");
        if (HHVM_REPO_LOCAL_MODE != NULL) {
          RepoLocalMode = HHVM_REPO_LOCAL_MODE;
        }
      }
      if (RepoLocalMode.empty()) {
        RepoLocalMode = "r-";
      }
      if (RepoLocalMode.compare("rw")
          && RepoLocalMode.compare("r-")
          && RepoLocalMode.compare("--")) {
        Logger::Error("Bad config setting: Repo.Local.Mode=%s",
                      RepoLocalMode.c_str());
        RepoLocalMode = "rw";
      }
      // Repo.Local.Path.
      RepoLocalPath = repoLocal["Path"].getString();
      if (!empty && RepoLocalPath.empty()) {
        const char* HHVM_REPO_LOCAL_PATH = getenv("HHVM_REPO_LOCAL_PATH");
        if (HHVM_REPO_LOCAL_PATH != NULL) {
          RepoLocalPath = HHVM_REPO_LOCAL_PATH;
        }
      }
    }
    {
      Hdf repoCentral = repo["Central"];
      // Repo.Central.Path.
      RepoCentralPath = repoCentral["Path"].getString();
    }
    {
      Hdf repoEval = repo["Eval"];
      // Repo.Eval.Mode.
      RepoEvalMode = repoEval["Mode"].getString();
      if (RepoEvalMode.empty()) {
        RepoEvalMode = "readonly";
      } else if (RepoEvalMode.compare("local")
                 && RepoEvalMode.compare("central")
                 && RepoEvalMode.compare("readonly")) {
        Logger::Error("Bad config setting: Repo.Eval.Mode=%s",
                      RepoEvalMode.c_str());
        RepoEvalMode = "readonly";
      }
    }
    RepoCommit = repo["Commit"].getBool(true);
    RepoDebugInfo = repo["DebugInfo"].getBool(true);
    RepoAuthoritative = repo["Authoritative"].getBool(false);
  }
  {
    Hdf sandbox = config["Sandbox"];
    SandboxMode = sandbox["SandboxMode"].getBool();
    SandboxPattern = Util::format_pattern
      (sandbox["Pattern"].getString(), true);
    SandboxHome = sandbox["Home"].getString();
    SandboxFallback = sandbox["Fallback"].getString();
    SandboxConfFile = sandbox["ConfFile"].getString();
    SandboxFromCommonRoot = sandbox["FromCommonRoot"].getBool();
    SandboxDirectoriesRoot = sandbox["DirectoriesRoot"].getString();
    SandboxLogsRoot = sandbox["LogsRoot"].getString();
    SandboxCheckMd5 = sandbox["CheckMd5"].getBool(false);
    if (!SandboxMode || RecordCodeCoverage) SandboxCheckMd5 = false;
    sandbox["ServerVariables"].get(SandboxServerVariables);
  }
  {
    Hdf mail = config["Mail"];
    SendmailPath = mail["SendmailPath"].getString("sendmail -t -i");
    MailForceExtraParameters = mail["ForceExtraParameters"].getString();
  }
  {
    Hdf preg = config["Preg"];
    PregBacktraceLimit = preg["BacktraceLimit"].getInt32(100000);
    PregRecursionLimit = preg["RecursionLimit"].getInt32(100000);
    EnablePregErrorLog = preg["ErrorLog"].getBool(true);
  }

  Extension::LoadModules(config);
  if (overwrites) Loaded = true;
}

///////////////////////////////////////////////////////////////////////////////
}
