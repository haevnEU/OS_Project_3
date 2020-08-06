print("DiskCreatorScriptV1");
createDisk("/mnt/d/trash/disk10.vdf",5368700);
listDisks();
selectDisk(0);
mount();
listDisks();
unmount();