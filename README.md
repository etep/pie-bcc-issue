# pie-bcc-issue
Demonstration of an issue where BCC cannot attach uprobes to PIE binaries.

## Usage.
```
docker build . -t issue-demo
docker run --privileged issue-demo /work/good
docker run --privileged issue-demo /work/fail
```

## Description of the issue.
For PIE binaries, BCC fails to attach uprobes when the attach point is provided as a virtual address.
