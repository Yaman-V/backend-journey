package com.taskmanager.api.controller;

import com.taskmanager.api.model.Task;
import com.taskmanager.api.service.TaskService;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
public class TaskController {

    // 1. Declare it as a private final variable
    private final TaskService taskService;

    // 2. Pass it into the constructor.
    // Spring is smart enough to find its Service Bean and plug it in here automatically.
    public TaskController(TaskService taskService) {
        this.taskService = taskService;
    }

    @GetMapping("/tasks")
    public List<Task> getAllTasks() {
        // 3. Now we use the injected instance, NO 'new' keyword!
        return taskService.getAllTasks();
    }
    @GetMapping("tasks/{title}")
    public List<Task> getTaskByTitle(@PathVariable String title){
        return taskService.getTaskByTitle(title);
    }
    @PostMapping("/tasks")
    public Task addTask(@RequestBody Task task){
        return taskService.addTask(task);
    }
    @DeleteMapping("/tasks/{title}")
    public boolean deleteTask(@PathVariable String title){
        return taskService.deleteTask(title);
    }
}