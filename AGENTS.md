# AGENTS.md — CUIFramework Development Standards

## Core Principles

### 1. Development Speed (快)
- **代码修改到生效 < 2秒** — 热重载必须在保存文件后立即生效
- **构建增量编译** — 只重新编译修改的文件，不做全量构建
- **最小依赖** — 依赖越少，构建越快，调试越简单
- **模块化架构** — core/ui/guest 三层分离，修改 UI 代码不影响核心层

### 2. Quality (准)
- **类型安全** — 使用 C++23 的 `std::expected` 处理错误，不用裸指针
- **编译期检查** — 模板元编程和 constexpr 在编译期捕获错误
- **声明式 API** — Builder 模式减少样板代码，降低出错概率
- **热重载状态保持** — `CR_STATE` 宏自动保存/恢复静态变量，避免状态丢失

### 3. Stability (稳)
- **cr.h 崩溃保护** — Guest DLL 崩溃不会影响 Host 进程
- **渐进式重载** — 修改后先验证再应用，避免级联错误
- **回滚机制** — 保留上一个稳定版本的 DLL，出错时自动回退
- **跨平台一致性** — 同一套代码在 Windows/macOS/Linux 行为一致

## Hot Reload 规范

### 必须遵守的规则
```
1. Guest DLL 必须导出以下函数：
   - cui_init(CUIContext*)     — 初始化
   - cui_update(CUIContext*)   — 每帧调用，返回 0 继续
   - cui_shutdown(CUIContext*) — 关闭

2. 只能使用 CR_STATE 标记的静态变量进行状态保存
   static int counter CR_STATE = 0;  // ✅ 正确
   static int counter = 0;           // ❌ 错误，重载后丢失

3. Guest 中不能使用以下功能（属于 Host 层）：
   - SDL_Init / SDL_CreateWindow
   - OpenGL 初始化代码
   - 文件 I/O（除读取资源）
   - 全局对象构造函数

4. 热重载触发条件：
   - 文件保存（文件系统监视）
   - 手动触发（按 F5 或调用 cr_plugin_update）
   - 定时轮询（每 500ms 检查一次）
```

### 性能要求
| 指标 | 目标 | 最大值 |
|------|------|--------|
| 热重载延迟 | < 500ms | 2s |
| 帧率 (UI 渲染) | 60 FPS | 30 FPS |
| 内存占用 (Guest) | < 50MB | 100MB |
| 构建时间 (增量) | < 3s | 10s |

## Architecture Standards

### 目录结构
```
CUIFramework/
├── core/          # 核心层 — 永不重载
│   ├── src/       # 实现文件
│   └── include/   # 公共头文件
├── ui/            # UI 引擎 — 静态链接到 Guest
│   ├── src/
│   └── include/
├── guest/         # Guest 层 — 热重载
│   ├── src/       # 用户 UI 代码
│   └── include/   # Guest API
└── examples/      # 示例代码
```

### 代码规范
1. **命名**: `snake_case` (变量/函数), `PascalCase` (类/类型)
2. **头文件**: `#pragma once`，不用 include guard
3. **C++23 特性**: 优先使用 `std::format`, `std::print`, `std::expected`
4. **内存管理**: 优先栈分配，需要时用 `std::unique_ptr`
5. **错误处理**: Guest 层返回错误码，Host 层用 `std::expected`

### 构建规范
```bash
# 增量构建（日常开发）
cmake --build --preset win-x64-debug

# 完整构建（发布前）
cmake --build --preset win-x64-release --clean-first

# 运行测试
ctest --preset win-x64-debug
```

## Hot Update 规范

### 远程更新机制
```
1. 版本检查:
   - 启动时检查服务器上的 manifest.json
   - 对比版本号，决定是否下载更新

2. 下载策略:
   - 增量更新：只下载修改的 DLL
   - 全量更新：版本不兼容时下载完整包
   - 后台下载：不阻塞主线程

3. 应用更新:
   - 下载完成后，等待用户确认或自动重启
   - 保留旧版本 DLL 作为回滚备份
   - 更新 manifest.json 标记新版本

4. 回滚机制:
   - 新版本出错时自动回退到旧版本
   - 用户可手动选择版本回退
```

### 更新包结构
```
update/
├── manifest.json      # 版本信息和文件列表
├── cui_guest.dll      # 更新后的 Guest DLL
├── cui_guest.pdb      # 调试符号（可选）
└── changelog.txt      # 更新说明
```

## Development Workflow

### 日常开发流程
```
1. 修改 guest/src/ui_main.cpp
2. 保存文件
3. 热重载自动触发（< 500ms）
4. 在窗口中查看效果
5. 不满意？继续修改，重复步骤 1-4
```

### 发布流程
```
1. 运行完整测试套件
2. 切换到 Release 构建
3. 运行性能基准测试
4. 打包（CPack 生成安装程序）
5. 上传到服务器
6. 通知用户更新
```

## Performance Optimization

### 必须遵守
1. **批处理渲染** — 一次性提交多个 draw call
2. **纹理图集** — 字体和图片合并到图集，减少纹理切换
3. **视口裁剪** — 只渲染可见区域
4. **帧率限制** — 默认 60 FPS，避免空转
5. **内存池** — 预分配内存，减少 malloc/free

### 可选优化
1. **多线程渲染** — UI 计算和渲染分离
2. **GPU 加速** — 使用 Compute Shader 做布局计算
3. **延迟渲染** — 先渲染到 FBO，再后处理

## Quality Assurance

### 测试要求
- **单元测试**: 覆盖率 > 80%
- **集成测试**: 每个组件都有集成测试
- **热重载测试**: 验证状态保持和崩溃恢复
- **跨平台测试**: Windows/macOS/Linux 至少各跑一遍

### 代码审查
- 每个 PR 必须有至少 1 人审查
- 热重载相关代码必须有额外审查
- 性能关键代码必须有 benchmark

## Emergency Procedures

### 热重载失败
```
1. 检查 Guest DLL 是否编译成功
2. 检查 CR_STATE 是否正确标记
3. 检查是否有未初始化的指针
4. 尝试手动触发重载（F5）
5. 如果仍然失败，重启 Host 进程
```

### 性能问题
```
1. 使用 profiler 定位瓶颈
2. 检查是否有不必要的内存分配
3. 检查 draw call 数量是否过多
4. 检查是否有重复渲染
5. 考虑降低渲染质量或帧率
```

---

**最后更新**: 2026-07-08
**版本**: 0.1.0
