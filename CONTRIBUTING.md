# Overview

- [Introduction](#introduction)
  - [Contributing to SPMod](#contributing-to-spmod)
  - [How can I contribute?](#how-can-i-contribute)
  - [Code of Conduct](#code-of-conduct)
- [Your First Contribution](#your-first-contribution)
  - [First contribution to an open source project](#first-contribution-to-an-open-source-project)
- [Getting started](#getting-started)
  - [Submitting contribution](#submitting-contribution)
- [How to report a bug](#how-to-report-a-bug)
  - [Security issues](#security-issues)
  - [Before filing a bug report](#before-filing-a-bug-report)
  - [The same bug has already been reported](#the-same-bug-has-already-been-reported)
- [How to suggest a feature or enhancement](#how-to-suggest-a-feature-or-enhancement)
  - [Goals](#goals)
  - [Process for suggesting a feature](#process-for-suggesting-a-feature)
- [Code review process](#code-review-process)
  - [Reviewers](#reviewers)
  - [Feedback](#feedback)
  - [Becoming a collaborator](#becoming-a-collaborator)
  - [TC](#tc)
- [Community](#community)
- [Extra notes](#extra-notes)
  - [Pull request and issues labels](#pull-request-and-issues-labels)
  - [Code style](#code-style)
    - [General](#general)
    - [Naming conventions](#naming-conventions)
      - [Variables & functions](#variables--functions)
      - [Classes, structures, filenames, unions](#classes-structures-filenames-unions)
    - [Function's parameters](#functions-parameters)
    - [Brackets](#brackets)
    - [auto keyword](#auto-keyword)
      - [Type can be easily determined](#type-can-be-easily-determined)
      - [Loops](#loops)
      - [Lambdas](#lambdas)

# Introduction

### Contributing to SPMod

You are here to contribute to SPMod? We would like to thank you first for considering contributing to SPMod. Please read the following sections to get to know how to contribute to the project. Following these guidelines ease maintainers' work on reviewing your issues and pull requests. It also shows that you respect their time spending on working on this project.

### How can I contribute?

There is plenty of ways to contribute to the project, f.e. by improving documentation, writing tutorials, submitting bug reports, fixing existing bugs, coding new features or improving current code base. Any of those will be valuable to SPMod itself.

### Code of Conduct
All members of our community are expected to follow our [Code of Conduct](https://github.com/Amaroq7/SPMod/blob/master/CODE_OF_CONDUCT.md). Make sure you do not violate it.

# Your First Contribution
If you want to help out with SPMod development, but you are unsure where to begin, you can start looking for beginner issues. They can help you dive into project and get familiar with its code base, coding style and so on. Those issues usually require little knowledge about the project and a few lines of code.

### First contribution to an open source project
If you found an issue you would like to work on, but do not know how to start, then you may want to check out:
- [How to Contribute to an Open Source Project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github)

Good, now you should be able to make your own changes! If there is something you do not understand well, feel free to ask for help. Keep in mind that everyone used to be a beginner at some point.

If you get call for "rebase" your PR by a maintainer, it means a lot of code has changed and to make merging easier you need to update your branch.

# Getting started
### Submitting contribution
1. Create your own fork of the project
2. Do the changes in your fork
3. If you like the change and think the project could use it:
    * Be sure you have followed the code style for the project,
    * Be sure you have not introduced any new bugs or regressions,
    * Document your changes,
    * Tests your changes.
4. Create a pull request using [template](https://github.com/Amaroq7/SPMod/blob/master/PULL_REQUEST_TEMPLATE.md)

# How to report a bug
### Security issues
If you find a security vulnerability, do not open an issue. Email karolsz9898@gmail.com instead.

In order to determine whether you are dealing with a security issue, ask yourself these two questions:
* Can I access something that is not mine, or something I should not have access to?
* Can I disable something for other people?

If the answer to one of those two questions is "yes", it is highly likely that is a security issue. Even though if the answer is "no" to them, it still may be a security issue, so should you have any doubts, email karolsz9898@gmail.com.

### Before filing a bug report
- Make sure if your bug is really a bug
- Check whether someone has already reported the same bug
- Make sure you are using up to date version
- Test under what conditions the bug is reproducible

If you have done all steps above, you can file a bug report. To do so, fill out the [issue template](https://github.com/Amaroq7/SPMod/blob/master/ISSUE_TEMPLATE.md).

### The same bug has already been reported
If that is the case, please check the report and ensure if every piece of information has been provided. If you believe there is something missing, please share your thoughts with us, it may help us fix the issue or speed up finding the cause of it.

# How to suggest a feature or enhancement
### Goals
- Ease server's administration
- Provide API for modules
- Provide API for plugins using [SourcePawn](https://github.com/alliedmodders/sourcepawn) to work with C(++) structures, classes, functions
- Be written using C11 and C++17 standards

### Process for suggesting a feature

If you feel there is a missing feature which other members of community can benefit from, feel free to open an issue. Describe the feature you would like to see, why you need it and how it should work.

# Code review process
No pull request can be merged without being reviewed. That rule also applies to every collaborator and TC member.

### Reviewers
Collaborators are responsible for reviewing pull requests.

### Feedback
Pull requests are checked on daily basis. After submitting your pull request you need to be patient from now on, time to review can vary on what your changes are about.

After collaborator's feedback you are obligated to make requested changes if any, otherwise the pull request may be closed as inactive. When all addressed issues by the collaborator has been resolved, pull request can be merged by any collaborator.

Pull requests are merged when no collaborator has raised objections to it, otherwise all involved collaborators should seek a consensus by expressing their concerns in discussion, find a compromise on or withdrawal the proposed change.

Whether the change introduced by the pull request is controversial and the compromise can not be found among collaborators, it is passed on the TC. It is expected that only small number of issues is passed on the TC and seeking consensus should be default action among collaborators.

## Becoming a collaborator
Every contributor who has made non-trivial changes to the project will be added as collaborator and get write access to the repository.

## TC
TC consists of collaborators which are nominated by other collaborators. TC uses its consensus seeking process to accept or rejected a nominee for TC as for issues which have been passed on it. TC tries to find resolution which has no objections among the members of the group. If reaching consensus is impossible then majority wins vote is called, but that is the last-resort option.

# Community
The core team is [@Amaroq7](https://github.com/Amaroq7). Everyone is welcomed to participate in SPMod's development.

You can chat with the core team on https://gitter.im/SPMod.

# Extra notes

### Pull request and issues labels

- Issues
  - **bug** - Anything that is broken
  - **regression** - A bug that did not exist in previous versions and isn't a new feature
  - **enhancement** - Feature request
  - **duplicate** - Already reported
  - **good first issue** - Less complex issue, good for first contribution
  - **wontfix** - Not a bug
  - **help wanted** - Help of community is appreciated to resolve the issue
  - **invalid** - Not valid issue, e.g. misusing api
  - **question** - Question, e.g. How to do X?
- Categories
  - **documentation** - Related to the documentation
  - **linux** - Related to the SPMod running on Linux
  - **windows** - Related to the SPMod running on Windows
  - **api** - Related to SPMod's public API
  - **meson** - Related to build system
- Pull requests
  - **work-in-progress** - Not complete PR, the work on it is still pending
  - **needs-review** - PR needs to be reviewed and approved by collaborators
  - **under-review** - PR is being reviewed
  - **requires-changes** - PR needs to be updated based on review
  - **needs-testing** - PR needs to be tested

### Code style
#### General
- tabs for indentation (length: 4),
- line's length can not exceed 120 characters.

#### Naming conventions

##### Variables & functions
SPMod uses camelCase convention for naming variables and functions.

For variables inside classes, structures, unions add `m_` before their names.

For private functions add `_` (underscore) before their names.

###### Examples
```cpp
class Forward
{
public:
    // Good
    std::string m_name;
    std::string m_pluginIdentity;

    // Bad
    std::string Name;
    std::string PluginIdentity;
    std::string m_Name;

private:
    // Good
    std::string_view _getName()
    {
        return m_name;
    }
    // Bad
    std::string_view getName()
    {
        return Name;
    }
};
void getId()
{
    // Good
    int pluginId;

    // Bad
    int PluginId;
    int pluginid;

    // ...
}
```

##### Classes, structures, filenames, unions
For naming those use PascalCase.

###### Examples
```cpp
// Good
class Forward
{
    // ...
};
struct NativeDef
{
    // ...
};

// Bad
class forward
{
    // ...
};
struct nativeDef
{
    // ...
};
```

#### Function's parameters

If function has more then 2 parameters, then each of them should be placed in a new line.

###### Examples
```cpp
// Good
int foo(size_t a,
        std::string b)
{
    // ...
}

// Bad
int foo(size_t a, std::string b)
{
    // ...
}
```

#### Brackets

Brackets must be placed in a new line.

```cpp
// Good
if (a)
{
    int b;
    float c;
}

// Exception: Omit brackets if "if block" is one lined
if (a)
    return;

// Function's body must be always wrapped in the brackets
void foo()
{
    // ...
}

// Bad
if (a) {
    int b;
}

void foo() {
    // ...
}
```

#### `auto` keyword

Should be used whenever it makes code easier to read.

##### Type can be easily determined

```cpp
// Good
auto ptr = std::make_shared<T>();
auto a = static_cast<U>(some_var);

// Bad
auto b = someFunc();
auto c = 0;
```

##### Loops
```cpp
for (auto iter = container.begin(); iter != container.end(); ++iter)
{
	// ...
}

for (auto &obj : container)
{
	// ...
}

for (const auto &obj : container)
{
	// ...
}
```

##### Lambdas
```cpp
auto someLambdaFunc = [](int a, int b)
{
    // ...
}
```
