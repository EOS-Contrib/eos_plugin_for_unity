# <div align="center">Preventing Code Stripping for Newtonsoft.Json</div>
---
## Overview

Some EOS samples and certain custom implementations may rely on **Newtonsoft.Json** for JSON serialization and deserialization.  
When building with **IL2CPP** and **Managed Code Stripping** enabled, Unity may remove unused or reflection-referenced members from assemblies.  
Because Newtonsoft.Json uses reflection heavily, this can lead to missing types at runtime (especially in **Android**, **iOS**, and **WebGL** builds).

This section describes how to **prevent code stripping** and ensure consistent runtime behavior.

> [!IMPORTANT]
> The `link.xml` file must be placed **inside your project’s `Assets/` directory**, otherwise it will be ignored.
---

## When This Applies

You should perform this step if **any of the following are true**:

* Your project uses **Newtonsoft.Json** directly (e.g., `JsonConvert.SerializeObject`, `JsonConvert.DeserializeObject`).
* You are using **EOS sample scenes** that depend on Newtonsoft.Json.
* You are building to **Android**, **iOS**, **WebGL**, or any platform using **IL2CPP**.

---

## Steps to Prevent Stripping

1. In your Unity project in Assets folder, create a new file link.xml
2. Add the following content:

```xml
<linker>
  <assembly fullname="Newtonsoft.Json" preserve="all" />
</linker>
```

> [!NOTE]  
>If your project conditionally includes Newtonsoft.Json, or you want to avoid warnings when the assembly is not present, you may use:
```xml
<linker>
  <assembly fullname="Newtonsoft.Json" preserve="all" ignoreIfMissing="1" />
</linker>
```

If size is critical, you may scope preservation to specific types/members instead of preserve="all". See [Unity’s Link XML reference](https://docs.unity3d.com/6000.2/Documentation/Manual/managed-code-stripping-xml-formatting.html?).